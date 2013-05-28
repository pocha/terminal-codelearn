/**
 * Siege environment initialization.
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
#include <init.h>
#include <setup.h>
#include <base64.h>
#include <util.h>
#include <fcntl.h>
char seti[128];

int
init_config( void )
{
  char *e;
  char filename[256];
  char buf[256];
  memset( &my,  0, sizeof( struct CONFIG )); /* from setup.h */
  memset( &our, 0, sizeof( struct STATUS )); /* from setup.h */
  my.signaled = 0;
  our.shutting_down = 0;
  our.total_threads = 0;

  if(( e = getenv("SIEGERC")) != NULL){
    snprintf(filename, sizeof( filename), e);
  }
  else{
    snprintf( filename, sizeof( filename), "%s/.siegerc", getenv( "HOME" ));
  }

  strcpy( my.file, SIEGE_HOME ); 
  strcat( my.file, "/etc/urls.txt" );
  my.debug     = FALSE;
  my.config    = FALSE;
  my.cookies   = TRUE;
  my.secs      = -1;
  my.reps      = MAXREPS; 
  my.authorize = FALSE;
  my.proxy     = FALSE;
  my.login     = FALSE;
  my.failures  = 20;
  my.failed    = 0;
  my.proxyport = 80; 
  my.timeout   = 30;
  my.extra[0]  = 0;
  my.follow    = TRUE;
  my.zero_ok   = FALSE; 
  if( load_conf( filename ) < 0 ){
    fprintf( stderr, "****************************************************\n" );
    fprintf( stderr, "siege: could not open %s\n", filename );
    fprintf( stderr, "run \'siege.config\' to generate a new .siegerc file\n" );
    fprintf( stderr, "****************************************************\n" );
    return -1;
  }

  if( strlen(my.uagent) < 1 ) 
    snprintf( 
      my.uagent, sizeof( my.uagent ),
      "JoeDog/1.00 [en] (X11; I; Siege %s)", version_string 
    );
  if( strlen(my.logfile) < 1 ) 
    snprintf( 
      my.logfile, sizeof( my.logfile ),
      "%s", LOG_FILE 
    );
  if(( my.username && strlen(my.username) > 0 ) &&
    (  my.password && strlen(my.password) > 0 )){
    snprintf( buf, sizeof( buf ), "%s:%s", my.username, my.password ); 
    if(( base64_encode( buf, strlen( buf ), &my.auth ) >= 0 )){
      my.authorize = TRUE;
    }
  }
  
  if( my.proxysrv && strlen(my.proxysrv) > 0 ){
    my.proxy = TRUE;
  } 

  return 0;  
}

int
show_config( int EXIT )
{
  int x;
  printf( "CURRENT  SIEGE  CONFIGURATION\n" );
  printf( "%s\n", my.uagent ); 
  printf( "Edit the resource file to change the settings.\n" );
  printf( "----------------------------------------------\n" );
  printf( "version:                        %s\n", version_string );
  printf( "verbose:                        %s\n", my.verbose?"true":"false" );
  printf( "debug:                          %s\n", my.debug?"true":"false" );
  printf( "protocol:                       %s\n", my.protocol?"HTTP/1.1":"HTTP/1.0" );
  if( my.proxy ){
    printf( "proxy-host:                     %s\n", my.proxysrv );
    printf( "proxy-port:                     %d\n", my.proxyport );
  }
  printf( "connection:                     %s\n", my.keepalive?"keep-alive":"close" );
  printf( "concurrent users:               %d\n", my.cusers );
  if( my.secs > 0 )
    printf( "time to run:                    %d seconds\n", my.secs );
  else
    printf( "time to run:                    n/a\n" );
  if(( my.reps > 0 )&&( my.reps != MAXREPS ))
    printf( "repetitions:                    %d\n", my.reps );
  else
    printf( "repetitions:                    n/a\n" );
  printf( "delay:                          %d sec%s\n", my.delay,my.delay>1?"s":"" );
  printf( "internet simulation:            %s\n", my.internet?"true":"false"  );
  printf( "benchmark mode:                 %s\n", my.bench?"true":"false"  );
  printf( "failures until abort:           %d\n", my.failures );
  printf( "named URL:                      %s\n", my.url==NULL?"none":my.url );
  printf( "URLs file:                      %s\n", 
    !strcasecmp(my.file, seti)?(strlen(seti)>0?seti:CNF_FILE):CNF_FILE );
  printf( "logging:                        %s\n", my.logging?"true":"false" );
  printf( "log file:                       %s\n", my.logfile==NULL?LOG_FILE:my.logfile );
  if(( getenv("SIEGERC")) != NULL){
    printf( "resource file:                  %s\n", getenv("SIEGERC"));
  } 
  else{
    printf( "resource file:                  %s/.siegerc\n", getenv( "HOME" ));
  }
  if( my.username!=NULL ) {
    printf( "username:                       %s\n", my.username);
  } 
  if( my.password!=NULL ){
    printf( "password:                       ");
    for( x = 0; x < strlen( my.password ); x++ )
      printf( "*" );
    printf( "\n" );
  }
  printf( "allow redirects:                %s\n", my.follow?"true":"false" );
  printf( "allow zero byte data:           %s\n", my.zero_ok?"true":"false" ); 
  printf( "\n" );

  if( EXIT ) exit( 0 );
  else return 0;
}

/**
 * chops the white space from
 * the beginning of a char *
 */
#if 0
static char
*chomp( char *str )
{
  while( *str == ' ' || *str == '\t') {
    str++;
  }
  return str;
}   
#endif

static char
*get_line( FILE *fp )
{
  char *ptr;
  char *new;
  char tmp[256];
 
  ptr = malloc( 1 );
 
  do{
    if(( fgets( tmp, sizeof( tmp ), fp )) == NULL ) return( NULL );
    if( ptr == NULL ) ptr = strdup( tmp );
    else{
      if(( ptr = xrealloc( ptr, strlen(ptr) + strlen(tmp) + 1 )) == NULL ) break;
      strcat( ptr, tmp );
    }
  } while(( new = strchr( ptr, '\n')) == NULL );
  if( new != NULL ) *new = '\0';
 
  return ptr;
} 

static char
*chomp_line( FILE *fp, char **mystr, int *line_num )
{
  char *ptr;
  while( TRUE ){
    if(( *mystr = get_line( fp )) == NULL) return( NULL );
    (*line_num)++;
    ptr = chomp( *mystr );
    /* exclude comments */
    if( *ptr != '#' && *ptr != '\0' ){
      return( ptr );
    }
  }
} 

int
load_conf( char *filename )
{
  FILE *fp;
  int  fd;
  char temp[32]; 
  int  line_num = 0;
  char *line;
  char *option;
  char *value;
 
  if (( fp = fopen(filename, "r")) == NULL ) {
    return -1;
  } 

  while(( line = chomp_line( fp, &line, &line_num )) != NULL ){
    option = line;
    while( *line && !ISSPACE( (int)*line ) && !ISSEPARATOR( *line ))
      line++;
    *line++=0;
    while( ISSPACE( (int)*line ) || ISSEPARATOR( *line ))
      line++;
    value  = line;
    while( *line )
      line++;  
    if( !strncasecmp( option, "verbose", 7   )){
      if( !strncasecmp( value, "true", 4 ))
        my.verbose = TRUE;
      else
        my.verbose = FALSE;
    } 
    if( !strncasecmp( option, "logging", 7 )){
      if( !strncasecmp( value, "true", 4 ))
        my.logging = TRUE;
      else
        my.logging = FALSE;
    }
    if( !strncasecmp( option, "show-logfile", 12 )){
      if( !strncasecmp( value, "true", 4 ))
        my.shlog = TRUE;
      else
        my.shlog = FALSE;
    }
    if( !strncasecmp( option, "logfile", 7 )){
      strncpy( my.logfile, value, sizeof( my.logfile ));
    } 
    if( !strncasecmp( option, "cookies", 7 )){
      if( !strncasecmp( value, "true", 4 ))
        my.cookies = TRUE;
      else
        my.cookies = FALSE;
    }
    if( !strncasecmp( option, "concurrent", 10 )){
      my.cusers = atoi( value );
    } 
    if( !strncasecmp( option, "reps", 4 )){
      my.reps = atoi( value );
    }
    if( strlen(option) < 7 && !strncasecmp( option, "time", 4 )){
      parse_time( value );
    }
    if( !strncasecmp( option, "delay", 5 )){
      my.delay = atoi( value );
    }
    if( !strncasecmp( option, "timeout", 7 )){
      my.timeout = atoi( value );
    }
    if( !strncasecmp( option, "internet", 8 )){
      if( !strncasecmp( value, "true", 4 ))
        my.internet = TRUE;
      else
        my.internet = FALSE;
    }
    if( !strncasecmp( option, "benchmark", 9 )){
      if( !strncasecmp( value, "true", 4 )) 
        my.bench = TRUE;
      else
        my.bench = FALSE;
    }
    if( !strncasecmp( option, "debug", 5 )){
      if( !strncasecmp( value, "true", 4 ))
        my.debug = TRUE;
      else
        my.debug = FALSE;
    }
    if( !strncasecmp( option, "file", 5 )){
      strncpy( my.file, value, sizeof( my.file ));
      strncpy( seti, value, sizeof( seti ));
    }
    if( !strncasecmp( option, "url", 3 )){
      my.url = (char*)strdup( value );
    }
    if( !strncasecmp( option, "user-agent", 10 )){
      strncpy( my.uagent, value, sizeof( my.uagent ));
    }
    if( !strncasecmp( option, "username", 8 )){
      my.username = value;
    }
    if( !strncasecmp( option, "password", 8 )){
      my.password = value;
    }
    if( !strncasecmp( option, "connection", 10 )){
      if( !strncasecmp( value, "keep-alive", 10 ))
        my.keepalive = TRUE;
      else
        my.keepalive = FALSE; 
    }
    if( !strncasecmp( option, "protocol", 8 )){
      if( !strncasecmp( value, "HTTP/1.1", 8 ))
        my.protocol = TRUE;
      else
        my.protocol = FALSE; 
    }
    if( !strncasecmp( option, "proxy-host", 10 )){
      my.proxysrv = value;
    }
    if( !strncasecmp( option, "proxy-port", 10 )){
      my.proxyport = atoi( value );
    } 
    if( !strncasecmp( option, "login-url", 9 )){
      my.login = TRUE;
      my.loginurl = value;
    } 
    if( !strncasecmp( option, "failures", 10 )){
      my.failures = atoi( value );
    }
    if( !strncasecmp( option, "header", 6 )){
	int ll;
	if(!strchr(value,':')) joe_fatal("no ':' in http-header");
	if((strlen(value) + strlen(my.extra) + 3) > 512) joe_fatal("too many headers");
	strcat(my.extra,value);
	strcat(my.extra,"\015\012");
    }
    if( !strncasecmp( option, "follow-location", 15 )){
      if ( !strncasecmp( value, "true", 4 ))
        my.follow = TRUE;
      else
        my.follow = FALSE;
    }
    if( !strncasecmp( option, "zero-data-ok", 12 )){
      if( !strncasecmp( value, "true", 4 ))
        my.zero_ok = TRUE;
      else
        my.zero_ok = FALSE;
    } 

  } /* end of while line=chomp_line */

  return 0;
}

/**
 * don't be insulted, the author is the 
 * DS Module in question...   ;-)
 */ 
void
ds_module_check( void )
{
  if( my.bench ){ my.delay = 0; }
  if( my.secs > 0 && (( my.reps > 0 ) && ( my.reps != MAXREPS ))){
    joe_error("CONFIG conflict: selected time and repetition based testing" );
    fprintf( stderr, "defaulting to time-based testing: %d seconds\n", my.secs );
    my.reps = MAXREPS;
  }
}

