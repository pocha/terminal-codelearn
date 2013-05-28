/**
 * HTTP/HTTPS protocol support 
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
 */
#include <setup.h>
#include <http.h>
#include <cookie.h>
#include <string.h>

#define MAXFILE 10240

/**
 * returns int, ( < 0 == error )
 * formats and sends an HTTP/1.0 request
 */
void
SIEGEhttp_send( CONN *C, char *host, char *path )
{
  int rlen;
  char *protocol; 
  char *keepalive;
  char cookie[MAX_COOKIE_SIZE];
  char request[2048]; 

  /* Request path based on proxy settings */
  char fullpath[4096];
  if( my.proxy ){
    sprintf( fullpath, "%s://%s%s", C->prot == 0?"http":"https", host, path );
  } 
  else{
    sprintf( fullpath, "%s", path );
  }

  memset( cookie,  0, sizeof( cookie ));
  memset( request, 0, sizeof( request ));

  /* HTTP protocol string */
  protocol  = (my.protocol == TRUE)?"HTTP/1.1":"HTTP/1.0";
  keepalive = (my.keepalive == TRUE)?"keep-alive":"close";
  get_cookie( (int)pthread_self(), host, cookie ); 

  /* build a request string to
     pass to the server       */
  if( my.authorize ){
    rlen=snprintf(
      request, sizeof( request ),
      "GET %s %s\015\012"
      "Host: %s\015\012"
      "Authorization: Basic %s\015\012"
      "Cookie: %s\015\012"
      "Accept: */*\015\012"
      "Accept-Encoding: * \015\012"
      "User-Agent: %s\015\012%s"
      "Connection: %s\015\012\015\012",
      fullpath, protocol, host, my.auth, cookie, my.uagent, my.extra, keepalive
    );
  }
  else{ 
    rlen=snprintf(
      request, sizeof( request ),
      "GET %s %s\015\012"
      "Host: %s\015\012"
      "Cookie: %s\015\012"
      "Accept: */*\015\012"
      "Accept-Encoding: * \015\012"
      "User-Agent: %s\015\012%s"
      "Connection: %s\015\012\015\012",
      fullpath, protocol, host, cookie, my.uagent, my.extra, keepalive
    );
  }
  
  if( my.debug ){ printf("%s\n", request); fflush(stdout); }
  if( rlen<0 || rlen>sizeof(request)  ){ joe_fatal("http_send: request buffer overrun!"); }

  if(( SIEGEsocket_check( C, WRITE )) < 0 ){
    joe_error( "SIEGEsocket: not writeable" );
    return;
  } 

  SIEGEsocket_write( C, request, rlen );

  return;
}

/**
 * returns int, ( < 0 == error )
 * formats and sends an HTTP/1.0 request
 */
void
SIEGEhttp_post( CONN *C, char *host, char *path, char *data, size_t len )
{
  int  rlen;
  char request[2048]; 
  char *protocol; 
  char *keepalive;

  /* cookie value  */
  char cookie[MAX_COOKIE_SIZE];

  /* Request path based on proxy settings */
  char fullpath[4096];
  if( my.proxy ){
    sprintf( fullpath, "%s://%s%s", C->prot == 0?"http":"https", host, path );
  }
  else{
    sprintf( fullpath, "%s", path );
  }

  memset( cookie,  0, sizeof( cookie ));
  memset( request, 0, sizeof( request ));

  /* HTTP protocol string */
  protocol  = (my.protocol  == TRUE)?"HTTP/1.1":"HTTP/1.0";
  keepalive = (my.keepalive == TRUE)?"keep-alive":"close";
  get_cookie( (int)pthread_self(), host, cookie );
    
  /* build a request string to
     pass to the server       */
  if( my.authorize ){
    rlen=snprintf(
      request, sizeof( request ),
      "POST %s %s\015\012"
      "Host: %s\015\012"
      "Authorization: Basic %s\015\012"
      "Cookie: %s\015\012"
      "Accept: */*\015\012"
      "Accept-Encoding: * \015\012"
      "User-Agent: %s\015\012%s"
      "Connection: %s\015\012"
      "Content-type: application/x-www-form-urlencoded\015\012"
      "Content-length: %d\015\012\015\012"
      "%*.*s\015\012",
      fullpath, protocol, host, my.auth, cookie, my.uagent, my.extra, keepalive, len, len, len, data
    );
  }
  else{
    rlen=snprintf(
      request, sizeof( request ),
      "POST %s %s\015\012"
      "Host: %s\015\012"
      "Cookie: %s\015\012"
      "Accept: */*\015\012"
      "Accept-Encoding: * \015\012"
      "User-Agent: %s\015\012%s"
      "Connection: %s\015\012"
      "Content-type: application/x-www-form-urlencoded\015\012"
      "Content-length: %d\015\012\015\012"
      "%*.*s\015\012",
      fullpath, protocol, host, cookie, my.uagent, my.extra, keepalive, len, len, len, data
    );
  }

  if( my.debug ){ printf("%s\n", request); fflush(stdout); }
  if( rlen<0 || rlen>sizeof(request) ){ joe_fatal("http_post: request buffer overrun!"); }

  if(( SIEGEsocket_check( C, WRITE )) < 0 ){
    joe_error( "SIEGEsocket: not writeable" );
    return;
  } 

  SIEGEsocket_write( C, request, rlen );

  return;
}

/**
 * returns HEADERS struct
 * reads from http/https socket and parses
 * header information into the struct.
 */
HEADERS *
SIEGEhttp_read_headers( CONN *C, char *host )
{ 
  int  x;           /* while loop index      */
  int  n;           /* assign socket_read    */
  char c;           /* assign char read      */
  char line[1536];  /* assign chars read     */
  HEADERS *h;       /* struct to hold it all */
  h = (HEADERS*)malloc( sizeof(HEADERS));
  memset( h, 0, sizeof( HEADERS ));
  
  if(( SIEGEsocket_check( C, READ )) < 0 ){
    joe_error( "SIEGEsocket: not readable" );
    return NULL;
  } 

  h->redirection[0]=0;

  while( TRUE ){
    x = 0;
    memset( &line, 0, sizeof( line ));
    while(( n = SIEGEsocket_read( C, &c, 1 )) == 1 ){
      line[x] = c; 
      if(( line[0] == '\n' ) || ( line[1] == '\n' )){ 
        return h;
      }
      if( line[x] == '\n' ) break;
      x ++;
    }
    line[x]=0;
    /* strip trailing CR */
    if (x > 0 && line[x-1] == '\r') line[x-1]=0;
    if( strncasecmp( line, "http", 4 ) == 0 ){
      strncpy( h->head, line, 8 );
      h->code = atoi( line + 9 ); 
    }
    if( strncasecmp( line, "content-length: ", 16 ) == 0 ){ 
      h->length = atol( line + 16 ); 
    }
    if( strncasecmp( line, "set-cookie: ", 12 ) == 0 ){
      if( my.cookies ){
        memset( h->cookie, 0, sizeof( h->cookie ));
        strncpy( h->cookie, line+12, strlen( line ));
        add_cookie( (int)pthread_self(), host, h->cookie );
      }
    }
    if( strncasecmp( line, "connection: ", 12 ) == 0 ){
      if ( strncasecmp( line+12, "keep-alive", 10 ) == 0 ){
        h->keepalive = 1;
      }
      else if( strncasecmp( line+12, "close", 5 ) == 0 ){
        h->keepalive = 0;
      }
    }

    if( strncasecmp(line, "location: ", 10) == 0) {
      if (strlen(line) - 10 > sizeof(h->redirection) - 1) {
	joe_warning( "redirection URL too long, ignored");
      }
      else {
        strcpy(h->redirection, line+10);
      }  
    }

    if( n <  0 ){ 
      joe_error("SIEGEhttp_read_headers"); 
      return( NULL ); 
    } /* socket closed */
  } /* end of while TRUE */

  return h;
}

/**
 * returns int
 * reads a http/https socket
 * ( you know what I mean :)
 */
ssize_t
SIEGEhttp_read( CONN *C, int len )
{ 
  int  n;
  size_t bytes=0;
  char body[MAXFILE];

  if(( SIEGEsocket_check( C, READ )) < 0 ){
    joe_error( "SIEGEsocket: not readable" );
    return -1;
  } 

  memset( &body, 0, MAXFILE );  
  while( TRUE ){
    if(( n = SIEGEsocket_read( C, body, MAXFILE)) == 0 ){
      break;
    } 
    /* IGV: should be accumulating bytes read, not just 
       recording those in the last packet */
    bytes += n;
  }
  return( bytes );
}

