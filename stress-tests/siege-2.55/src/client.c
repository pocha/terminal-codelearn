/**
 * HTTP/HTTPS client support
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
 * 
 */
#include <setup.h>
#include <client.h>
#include <signal.h>
#include <sock.h>
#include <http.h>
#include <url.h>

/**
 * local prototypes
 */ 
int http_request( URL U, DATA *d ); 
void clean_up(); 
static void signal_handler( int sig );
static void signal_init();

/**
 * local variables
 */
static pthread_once_t once = PTHREAD_ONCE_INIT; 

static void
signal_handler( int sig )
{
  pthread_exit( NULL );
}
 
static void
signal_init()
{
  struct sigaction sa;
 
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGUSR1, &sa, NULL);
}

/**
 * the thread entry point for signal friendly clients, 
 * currently only HP-UX. Other OS users should use the
 * cancel_client function below...
 */
#if defined(hpux) || defined(WINDOWS) 
void *
signal_client( DATA *d )
{
  int x, y;                   /* loop counters, indices  */
  int type, state;            /* old cancel type & state */
  sigset_t  sigs;
 
 
  pthread_once(&once, signal_init);
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGUSR1);
  pthread_sigmask(SIG_UNBLOCK, &sigs, NULL);
 
  /**
   * lock and increment the thread counter
   */
  pthread_mutex_lock( &our.mutex_lock );
  if( our.shutting_down != TRUE ){
    our.total_clients ++;
    our.total_threads ++;
  }
  pthread_cond_signal(&our.exit_cond);
  pthread_mutex_unlock(&our.mutex_lock);
 
 
  if( my.login == TRUE ){
    /**
     * add_url is going to mungle the char pointer
     * so we dupe it to preserve the intergrity of
     * the shared my.loginurl
     */
    http_request( add_url( my.login ), d );
  }
 
  for( x = 0, y = 0; x < my.reps; x++, y++ ){
    x = (( my.secs > 0 ) && (( my.reps <= 0 )||( my.reps == MAXREPS))) ? 0 : x;
 
    if( my.internet ){
      y = my_random( my.length, (int)pthread_self());
    }
    else{
      y = ( y < my.length ) ? y : 0;
    }
 
    if(( http_request( urls[y], d ) == -1 )) my.failed++;
 
    if( my.failed >= my.failures ){
      break;
    }
  }
 
  return( NULL );
} /*END SIGNAL_CLIENT*/
#else 
/**
 * thread entry point for cancellable friendly
 * operating systems, aix, GNU/linux, solaris, bsd, etc.
 */
void * 
cancel_client( DATA *d )
{
  int x, y;                   /* loop counters, indices  */
  int type, state;            /* old cancel type & state */

#if defined(_AIX)
  pthread_cleanup_push( (void(*)(void*))clean_up, NULL ); 
#else
  pthread_cleanup_push( (void*)clean_up, NULL ); 
#endif

  /** 
   * lock and increment the thread counter 
   */
  pthread_mutex_lock( &our.mutex_lock );
  if( our.shutting_down != TRUE ){
    our.total_clients ++;
    our.total_threads ++;
  }
  pthread_mutex_unlock(&our.mutex_lock); 
  pthread_cond_signal(&our.exit_cond);

  /**
   * set the cancel type and cancel state 
   * by OS type: big issue async vs defer
   */
#if defined(sun)
  pthread_setcanceltype ( PTHREAD_CANCEL_DEFERRED, &type );
#elif defined(_AIX)
  pthread_setcanceltype ( PTHREAD_CANCEL_DEFERRED, &type );
#elif defined(hpux) 
  pthread_setcanceltype ( PTHREAD_CANCEL_ASYNCHRONOUS, &type );
#else
  pthread_setcanceltype ( PTHREAD_CANCEL_ASYNCHRONOUS, &type );
#endif
  pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, &state ); 

  if( my.login == TRUE ){
    /**
     * add_url is going to mungle the char pointer
     * so we dupe it to preserve the intergrity of
     * the shared my.loginurl
     */
    http_request( add_url( my.loginurl ), d );
  }

  for( x = 0, y = 0; x < my.reps; x++, y++ ){
    x = (( my.secs > 0 ) && (( my.reps <= 0 )||( my.reps == MAXREPS))) ? 0 : x;

    if( my.internet ){
      y = my_random( my.length, (int)pthread_self());
    }
    else{
      y = ( y < my.length ) ? y : 0;
    } 

    if(( http_request( urls[y], d ) == -1 )) my.failed++;

    if( my.failed >= my.failures ){ 
      break;
    }
  }

  /**
   * every cleanup must have a pop
   */
  pthread_cleanup_pop( 0 );
 
  return( NULL );
} /*END CANCEL_CLIENT*/
#endif

int
http_request( URL U, DATA *d )
{
  int okay = 0;               /* connection check        */
  int bytes, code, fail;      /* statistics quantifiers  */
  float etime;                /* trans. elapsed time     */
  clock_t start, stop;        /* time structs for elaps. */
  struct tms t_start, t_stop; /* time structs for elaps. */
  CONN    *C;                 /* connection structure    */
  HEADERS *head;              /* HTTP header structure   */ 
  URL redirect_url;           /* URL in redirection request */

  C = malloc( sizeof( CONN ));
  C->pos_ini  = 0;
  C->inbuffer = 0; 

  if( U.protocol == UNSUPPORTED ){ 
    if( my.verbose ){
      printf(
        "%s %d %6.2f secs: %7d bytes ==> %s\n",
        "UNSPPRTD", 501, 0.00, 0, "PROTOCOL NOT SUPPORTED BY SIEGE" 
      );
    } /* end if my.verbose */
  }

  if( my.delay ){
    pthread_sleep_np(
     (int) (((double)rand() / ((double)RAND_MAX + 1) * my.delay ) + .05)
    );
  }

  C->prot = U.protocol;

  /* record transaction start time */
  start = times( &t_start );  

  if( my.debug ){ 
    printf( 
      "attempting connection to %s:%d\n", 
      (my.proxy==TRUE)?my.proxysrv:U.hostname,
      (my.proxy==TRUE)?my.proxyport:U.port 
    ); 
    fflush( stdout ); 
  } 

  if( my.proxy ){
    C->sock = SIEGEsocket( C, my.proxysrv, my.proxyport );
  } 
  else{
    C->sock = SIEGEsocket( C, U.hostname, U.port );
  } 

  if( C->sock < 0 ){
    if( my.debug ){ printf( "connection failed.\n" ); fflush( stdout ); } 
    okay = 0;
    fail = 1;
    C->sock = 0;
  }  
  else{ okay = 1; }
  if(  !okay   ){ return -1; }
  if( my.debug ){ printf( "connention made.\n" ); fflush( stdout ); } 
 
  /**
   * write to socket with a POST or GET
   */
  if( U.calltype == URL_POST ) 
    SIEGEhttp_post( 
      C, U.hostname, U.pathname, U.postdata, U.postlen 
    );
  else
    SIEGEhttp_send( 
      C, U.hostname, U.pathname 
    );
  
  /**
   * read from socket and collect statistics.
   */
  head     =  SIEGEhttp_read_headers( C, U.hostname ); 
  if( ! head ){ SIEGEclose( C ); free( C ); return -1; } 
  bytes    =  SIEGEhttp_read( C, 0 ); 
  if( !my.zero_ok && (bytes < 1) ){ 
    SIEGEclose( C ); 
    free( C ); 
    free( head ); 
    return -1; 
  } 
  stop     =  times( &t_stop ); 
  etime    =  elapsed_time( stop - start );  
  code     =  (head->code <  400) ? 1 : 0;
  fail     =  (head->code >= 400) ? 1 : 0; 

  /**
   * quantify the statistics for this client.
   */
  d->bytes += bytes;
  d->time  += etime;
  d->code  += code;
  d->fail  += fail;
  d->hits  ++;

  /**
   * verbose output, print statistics to stdout
   */
  if(( my.verbose )&&( our.shutting_down != TRUE )){
    printf( 
      "%s %d %6.2f secs: %7d bytes ==> %s\n", 
      head->head, head->code, etime, bytes, U.pathname
    );
  }

  /**
   * close the socket and free memory.
   */
  SIEGEclose( C );
  free( C ); 
  
  /**
   * deal with redirect requests from server 
   */
  if( my.follow && head->code == 302 && head->redirection[0]){ 
    if( my.debug ){ printf("parsing redirection URL %s\n", head->redirection); }
    redirect_url = add_redirect( head->redirection, U.port );
    free(head);
    http_request( redirect_url, d );
  }     
  else{
    free( head );
  }

  /* NOTE: Join all static read threads */
  return 0;
}


/**
 * called by pthread_cleanup and invoked
 * on pthread_cancel. We de-increment the
 * thread total before exiting.
 */
void
clean_up()
{
  pthread_mutex_lock(&our.mutex_lock);
  our.total_threads --;
  pthread_cond_signal(&our.exit_cond);
  pthread_mutex_unlock(&our.mutex_lock); 
}


