/**
 * Signal Handling
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
#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#ifdef  HAVE_PTHREAD_H
# include <pthread.h>
#endif/*HAVE_PTHREAD_H*/ 

#include <setup.h>
#include <handler.h> 

void
spin_doctor()
{
  long x;
  char h[4] = {'-','\\','|','/'};

  for( x = 0; our.total_threads > 1 || x < 55; x++ ){
    fflush( stdout );
    fprintf(stdout,"%c", h[x%4] );
    pthread_usleep_np( 20000 );
    fprintf( stdout, "\b" ); 
  }
  return;
}

void *
sig_handler( pthread_t *peer )
{
  int gotsig = 0, x; 
  int result;
  sigset_t  sigs;
  pthread_t spinner;   
 
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGHUP);
  sigaddset(&sigs, SIGINT);
  sigaddset(&sigs, SIGTERM);
  sigprocmask(SIG_BLOCK, &sigs, NULL);

  /** 
   * Now wait around for something to happen ... 
   */
  sigwait(&sigs, &gotsig);
  fprintf( stderr, "\nLifting the server siege..." ); 
  pthread_mutex_lock( &our.mutex_lock );
  our.shutting_down = TRUE;
  pthread_mutex_unlock( &our.mutex_lock );

#if defined (hpux)
#else
  if( result = pthread_create( &spinner, NULL, (void*)spin_doctor, NULL ) < 0 ){
    joe_fatal( "failed to create handler: %d\n", result );
  }   
#endif

  /**
   * The signal consistently arrives early,
   * so we artificially extend the life of
   * the siege to make up the discrepancy. 
   */
  pthread_usleep_np( 501125 ); 
  
  for( x = 0; x < our.total_clients; x ++ )
#if defined(hpux)
    pthread_kill( peer[x], SIGUSR1 ); 
#else
    pthread_cancel( peer[x] ); 
#endif
 
  pthread_join( spinner, NULL );
  pthread_exit( NULL );
}

