/**
 * Siege, http regression tester / benchmark utility
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
 * 
 *
 */
#define  INTERN  1

#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#ifdef  HAVE_PTHREAD_H
# include <pthread.h>
#endif/*HAVE_PTHREAD_H*/

/*LOCAL HEADERS*/
#include <setup.h>
#include <handler.h>
#include <timer.h>
#include <client.h>
#include <getopt.h>
#include <util.h>
#include <log.h>
#include <init.h>
#include <cfg.h>
#include <url.h>
#include <ssl.h>
#include <cookie.h>
#include <sys/resource.h>

int fd         =    0;    /* tmp file, for user defined URL          */
int count      =    0;    /* count messages processes, ie. transacts */
int code       =    0;    /* count HTTP successes, i.e., <  400      */
int fail       =    0;    /* count HTTP failures,  i.e., >= 400      */ 
clock_t start, stop;      /* process start and stop times.           */
char temp[32];            /* container for tmp file name.            */

struct tms t_start, t_stop;

/**
 * long options, std options struct
 */
static struct option long_options[] =
{
  { "version",    no_argument,       NULL, 'V' },
  { "help",       no_argument,       NULL, 'h' },
  { "verbose",    no_argument,       NULL, 'v' },
  { "config",     no_argument,       NULL, 'C' },
  { "debug",      no_argument,       NULL, 'D' },
  { "concurrent", required_argument, NULL, 'c' },
  { "internet",   no_argument,       NULL, 'i' },
  { "benchmark",  no_argument,       NULL, 'b' },
  { "reps",       required_argument, NULL, 'r' },
  { "time",       required_argument, NULL, 't' },
  { "delay",      required_argument, NULL, 'd' },
  { "log",        no_argument,       NULL, 'l' },
  { "file",       required_argument, NULL, 'f' },
  { "url",        required_argument, NULL, 'u' },
  { "mark",       required_argument, NULL, 'm' },
  { "header",     required_argument, NULL, 'H' } 
};

/**
 * display_version   
 * displays the version number and exits on boolean false. 
 * continue running? TRUE=yes, FALSE=no
 * return void
 */
void 
display_version( int i )
{
  /**
   * version_string is defined in version.c 
   * adding it to a separate file allows us
   * to parse it in configure.  
   */
  if( my.debug )
    printf( "Siege %s: debugging enabled\n", version_string );
  else 
    printf( "Siege %s\n", version_string );
  /**
   * if TRUE, exit 
   */
  if( i == 1 ){ exit( EXIT_SUCCESS ); }
}  /* end of display version */

/**
 * display_help 
 * displays the help section to STDOUT and exits
 */ 
void 
display_help()
{
  /**
   * call display_version, but do not exit 
   */
  display_version( FALSE ); 
  printf("Usage: siege [options]\n");
  printf("Options:\n"                    );
  puts("  -V, --version         VERSION, prints version number to screen.");
  puts("  -h, --help            HELP, prints this section.");
  puts("  -C, --config          CONFIGURATION, show the current configuration.");
  puts("  -v, --verbose         VERBOSE, prints notification to screen.");
  puts("  -c, --concurrent=NUM  CONCURRENT users, default is 10");
  puts("  -u, --url=\"URL\"       URL, a single user defined URL for stress testing." );
  puts("  -i, --internet        INTERNET user simulation, hits the URLs randomly." );
  puts("  -b, --benchmark       BENCHMARK, signifies no delay for time testing." );
  puts("  -t, --time=NUMm       TIME based testing where \"m\" is the modifier S, M, or H" );
  puts("                        no space between NUM and \"m\", ex: --time=1H, one hour test." );
  puts("  -r, --reps=NUM        REPS, number of times to run the test, default is 25" );
  puts("  -f, --file=FILE       FILE, change the configuration file to file." );
  puts("  -l, --log             LOG, logs the transaction to PREFIX/var/siege.log");
  puts("  -m, --mark=\"text\"     MARK, mark the log file with a string separator." );
  puts("  -d, --delay=NUM       Time DELAY, random delay between 1 and num designed" );
  puts("                        to simulate human activity. Default value is 3" );
  puts("  -H, --header=\"text\"   Add a header to request (can be many)" ); 
  /**
   * our work is done, exit nicely
   */
  exit( EXIT_SUCCESS );
}

/**
 * parses command line arguments and assigns
 * values to run time variables. relies on GNU
 * getopts included with this distribution.  
 */ 
void 
parse_cmdline( int argc, char *argv[] )
{
  int c = 0;
  while(( c = getopt_long( argc, argv, "VhvCDlibr:t:f:d:c:u:m:H:", 
          long_options, (int *)0)) != EOF ){
  switch( c ){
      case 'V':
        display_version( TRUE );
        break;
      case 'h':
        display_help();
        exit( EXIT_SUCCESS );
      case 'D':
        my.debug = TRUE;
        break;
      case 'C':
        my.config = TRUE;
        break;
      case 'c':
        my.cusers  = atoi( optarg );
        break;
      case 'i':
        my.internet = TRUE;
        break;
      case 'b':
        my.bench    = TRUE;
        break;
      case 'd':
        my.delay   = atoi( optarg );
        break;
      case 'l':
        my.logging = TRUE;
        break;
      case 'm':
        my.mark    = TRUE;
        my.markstr = optarg;
        my.logging = TRUE; 
        break;
      case 'v':
        my.verbose = TRUE;
        break;
      case 'r':
        my.reps = atoi( optarg );
        break;
      case 't':
        parse_time( optarg );
        break;
      case 'f':
        memset( my.file, 0, sizeof( my.file ));
        if( optarg == NULL ) break; /*paranoia*/
        strncpy( my.file, optarg, strlen( optarg ));
        break;
      case 'u':
        my.url = (char*)strdup( optarg );
        break;
      case 'H':
        {
          int ll;
          if(!strchr(optarg,':')) joe_fatal("no ':' in http-header");
          if((strlen(optarg) + strlen(my.extra) + 3) > 512)
              joe_fatal("too many headers");
          strcat(my.extra,optarg);
          strcat(my.extra,"\015\012");
        }
        break; 

    } /** end of switch( c )           **/
  }   /** end of while c = getopt_long **/
}     /** end of parse_cmdline         **/

/**
 * siege main
 */  
int 
main( int argc, char *argv[] )
{
  int x, result;                /* index, func. result*/
  float elapsed;                /* total elapsed time */
  float favail, fcount, ffailed;
  float ttime = 0.0;	        /* total time of test */
  unsigned long int bytes = 0;  /* total bytes trans. */
  LINES            *lines;      /* defined in setup.h */
  DATA             *data;       /* defined in setup.h */
  pthread_t        *peer;       /* array of pthreads  */
  pthread_t        cease;       /* the shutdown thread*/
  pthread_t        timer;       /* the timer thread   */
  pthread_t        spinner;     /* the spinner thread */
  pthread_attr_t   scope_attr;  /* set to PTHREAD_SCOPE_SYSTEM */
  void *statusp;
  sigset_t sigs;
  int gotsig = 0;
  
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGHUP);
  sigaddset(&sigs, SIGINT);
  sigaddset(&sigs, SIGALRM);
  sigaddset(&sigs, SIGTERM);
  sigprocmask(SIG_BLOCK, &sigs, NULL);
 
  lines = (LINES*) malloc(sizeof(LINES));
  lines->index   = 0;
  lines->line    = NULL;
 
  init_config();                /* defined in init.h */
  parse_cmdline(argc, argv);    /* defined above     */
  ds_module_check();            /* check config integ*/
  
  if( my.config ){
    /* see: init.h */  
    show_config( TRUE );    
  }

  if( my.url != NULL ){
    /* defined at cmdline */ 
    my.length = 1;  
  }
  else {
    /* defined in file   */  
    my.length = read_cfg_file ( lines, my.file );
  }

  if( my.length <= 2 ) my.internet = TRUE;
  if( my.length == 0 ) display_help();
  

  /* cookie is an EXTERN, defined in setup */ 
  cookie = (COOKIE*)malloc( sizeof(COOKIE)); 
  if( !cookie ){ joe_fatal( "memory exhausted" ); }
  cookie->first = NULL;
  if(( result = pthread_mutex_init( &(cookie->mutex), NULL)) !=0 ){
    joe_fatal( "pthread_mutex_init" );
  } 

  /* memory allocation for threads and data */
  urls = (URL*)malloc(my.length * sizeof(URL));
  if( !urls ){ joe_fatal( "application memory exhausted" ); }
  data = (DATA*)malloc(sizeof(DATA) * my.cusers);
  if( !data ){ joe_fatal( "application memory exhausted" ); }
  peer = (pthread_t*)malloc(sizeof(pthread_t) * my.cusers);
  if( !peer ){ joe_fatal( "application memory exhausted" ); }

  /* user added url from the cmd line */
 
  /** 
   * determine the source of the url(s),
   * command line or file, and add them
   * to the urls struct.
   */
  if( my.url != NULL ){
    urls[0]   =  add_url( my.url );       /* from command line  */
  }
  else{ 
    for( x = 0; x < my.length; x ++ ){
      urls[x] =  add_url(lines->line[x]); /* from urls.txt file */
    }
  }

  /**
   * display information about the siege
   * to the user and prepare for verbose 
   * output if necessary.
   */
  fprintf( stderr, "** " ); 
  display_version( FALSE );
  fprintf( stderr, "** Preparing %d concurrent users for battle.\n", my.cusers );
  fprintf( stderr, "The server is now under siege..." );
  if( my.verbose ){ fprintf( stderr, "\n" ); }

  /**
   * record start time before spawning threads
   * as the threads begin hitting the server as
   * soon as they are created.
   */
  start = times( &t_start );

  /**
   * for each concurrent user, spawn a thread and
   * loop until condition or pthread_cancel from the
   * handler thread.
   */
  pthread_attr_init( &scope_attr );
  pthread_attr_setscope( &scope_attr, PTHREAD_SCOPE_SYSTEM );
#if defined(_AIX)
  /* AIX, for whatever reason, defies the pthreads standard and  *
   * creates threads detached by default. (see pthread.h on AIX) */
  pthread_attr_setdetachstate( &scope_attr, PTHREAD_CREATE_JOINABLE );
#endif

  /** 
   * invoke OpenSSL's thread safety
   */
#ifdef HAVE_SSL
  SSL_thread_setup();
#endif/*

  /**
   * create the signal handler and timer;  the
   * signal handler thread (cease) responds to
   * ctrl-C (sigterm) and the timer thread sends
   * sigterm to cease on time out.
   */
  if( result = pthread_create( &cease, NULL, (void*)sig_handler, (void*)peer ) < 0 ){
    joe_fatal( "failed to create handler: %d\n", result );
  }
  if( my.secs > 0 ){
    if( result = pthread_create( &timer, NULL, (void*)siege_timer, (void*)cease ) < 0 ){
      joe_fatal( "failed to create handler: %d\n", result );
    } 
  }
  
  /**
   * loop until my.cusers and create a corresponding thread...
   */  
  for( x = 0; x < my.cusers && our.shutting_down != TRUE; x++ ){
    data[x].bytes = 0;
    data[x].time  = 0.0;
    data[x].hits  = 0;
    data[x].code  = 0;
    data[x].fail  = 0; 
#if defined(hpux) || defined(WINDOWS)
    result = pthread_create( &(peer[x]), &scope_attr, (void*)signal_client, &(data[x]) );
#else
    result = pthread_create( &(peer[x]), &scope_attr, (void*)cancel_client, &(data[x]) );
#endif
    if( result == EAGAIN ){ 
      my.verbose = FALSE;
      fprintf(stderr, "Unable to spawn additional threads; you may need to\n");
      fprintf(stderr, "upgrade your libraries or tune your system in order\n"); 
      fprintf(stderr, "to exceed %d users.\n", my.cusers );
      joe_fatal( "system resources exhausted" ); 
    }
    pthread_usleep_np( 1000 ); 
  } /* end of for pthread_create */

  /**
   * our.total_clients is the threads high water
   * mark; join all the threads that were spawned.
   */
  x = 0;
  do{
    pthread_join( peer[x++], &statusp );
    fflush( stdout );
  } while( x < our.total_clients ); 

#ifdef HAVE_SSL
  SSL_thread_cleanup();
#endif/*

  /**
   * collect all the data from all the threads that
   * were spawned by the run.
   */
  for( x = 0; x < our.total_clients; x++ ){
    count += data[x].hits;
    bytes += data[x].bytes;
    ttime += data[x].time;
    code  += data[x].code;
    fail  += data[x].fail;
  } /* end of stats accumulation */
  
  /**
   * record stop time
   */
  stop = times( &t_stop );

  /**
   * take a short nap  for  cosmetic  effect
   * this does NOT affect performance stats.
   */
  pthread_usleep_np( 10000 );
  fprintf( stderr, "\b      done.\n" );

  /**
   * prepare and print statistics.
   */
  ffailed = my.failed;
  fcount  = count;
  elapsed = elapsed_time( stop - start);
  favail  = (fcount==0)?0:((fcount/(fcount+ffailed))*100);
  if( my.failed >= my.failures ){
    printf("Siege aborted due to excessive socket failure; you\n");
    printf("can change the failure threshold in $HOME/.siegerc\n");
  }
  printf( "\bTransactions:\t\t%12d hits\n", count );
  printf( "Availability:\t\t%12.2f \%\n", favail);
  printf( "Elapsed time:\t\t%12.2f secs\n", elapsed );
  printf( "Data transferred:\t%12u bytes\n", bytes );
  printf( "Response time:\t\t%12.2f secs\n", ttime / count );
  printf( "Transaction rate:\t%12.2f trans/sec\n", count / elapsed );
  printf( "Throughput:\t\t%12.2f bytes/sec\n", bytes / elapsed );
  printf( "Concurrency:\t\t%12.2f\n", ttime / elapsed );
  printf( "Successful transactions:%12d\n", code ); 
  printf( "Failed transactions:\t%12d\n", fail+my.failed );
  puts  ( " " );

  if( my.mark )    mark_log_file( my.markstr );
  if( my.logging ) write_to_log( count, elapsed, bytes, ttime, code, (fail+my.failed));

  /**
   * exit program.
   */
  exit( EXIT_SUCCESS );	
} /* end of int main **/



