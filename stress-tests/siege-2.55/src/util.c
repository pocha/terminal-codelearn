/**
 * Utility Functions
 *
 * Copyright (C) 2000, 2001, 2002 by
 * Jeffrey Fulmer - <jdfulmer@armstrong.com>
 * This file is distributed as part of Siege 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * --
 *
 */
#include <setup.h>

/*
  parses the -t/--time option for a timed
  interval.  The option requires a modifier,
  H, M, or S, hours, minutes or seconds    */
void
parse_time( char *p )
{
  int x = 0;
  my.time = my.secs = 0;
  while( ISDIGIT( p[x] ))
    x++;
  my.time = atoi( substring( p, 0, x ));

  for( ; x < strlen( p ); x ++ )
    switch( TOLOWER( p[x] )){
      case 's':
        my.secs = my.time;
        my.time = 1;
        return;
      case 'm':
        my.secs = my.time * 60;
        my.time = 1;
        return;
      case 'h':
        my.secs = my.time * 3600;
        my.time = 1;
        return;
      default:
        break;
    }
  if(( my.time > 0 ) && ( my.secs <= 0 )){
    my.secs = my.time * 60;
  }

  return;
}

/**
 * sleep and usleep work on all supported
 * platforms except solaris, so we'll use
 * those functions on non-solaris, but we
 * still need to sleep. The macro handles
 * that for us.
 */
void
pthread_sleep_np( int secs )
{
  int seconds;

#if defined( SOLARIS ) || defined( sun )
  int err;
  time_t now;
  struct timespec timeout;
  pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t  timer_cond  = PTHREAD_COND_INITIALIZER;

  seconds = (secs<0)?0:secs;
  if( time(&now) < 0 ){ joe_fatal( "timer failed!" ); }
  timeout.tv_sec=now + seconds;
  timeout.tv_nsec=0;
 
  pthread_mutex_lock( &timer_mutex );
  err = pthread_cond_timedwait( &timer_cond, &timer_mutex, &timeout); 
#else
  seconds = (secs<0)?0:secs; 
  sleep( seconds );
#endif/*pthread_sleep_np*/

  return;
}


void
pthread_usleep_np( unsigned long usec )
{
  int seconds;

#if defined( SOLARIS ) || defined( sun ) 
  int err, type;
  struct timeval  now; 
  struct timespec timeout;
  pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t  timer_cond  = PTHREAD_COND_INITIALIZER;
  
  seconds = (usec<0)?0:usec;
 
  pthread_setcanceltype( PTHREAD_CANCEL_DEFERRED, &type ); 
  gettimeofday(&now, NULL);
  timeout.tv_sec  = now.tv_sec;
  timeout.tv_nsec = ( now.tv_usec * 1000) + ( seconds * 100 );
  if( timeout.tv_nsec >= 1000000000 ){
    timeout.tv_nsec -= 1000000000;
    timeout.tv_sec++;
  }

  pthread_mutex_lock( &timer_mutex );
  err = pthread_cond_timedwait( &timer_cond, &timer_mutex, &timeout);
  
  pthread_setcanceltype(type,NULL);
  pthread_testcancel();  
#else
  seconds = (usec<0)?0:usec;
  usleep( seconds );
#endif
  return;  
}


