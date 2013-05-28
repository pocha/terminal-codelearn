/**
 * Cookies Support
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
#include <cookie.h>

/**
 * insert values into list
 */
int
add_cookie( int id, char *host, char *cookiestr )
{
  char *lval, *rval;
  char *name, *value, *domain;
  size_t len;
  int  d = 0;  
  int  found = FALSE;
  CNODE *cur, *pre, *new = NULL; 
 
  /** 
   * DIRTY LAUNDRY  
   * grabs the first key value pair and stores 
   * it as name value in the linked list. It 
   * ignores expiration and path...
   */
  lval = cookiestr;
  while( *cookiestr && *cookiestr != '=' )
    cookiestr++;
  *cookiestr++=0;
 
  rval = cookiestr;
  while( *cookiestr && *cookiestr != ';' ) 
    cookiestr++;
  *cookiestr++=0;

  name  = (lval != NULL)?strdup( lval ):NULL;
  value = (rval != NULL)?strdup( rval ):NULL; 

  if(( name == NULL || value == NULL )) return -1;

  pthread_mutex_lock(&(cookie->mutex)); 
   
  for( cur=pre=cookie->first; cur != NULL; pre=cur, cur=cur->next ){
    if(( cur->threadID == id )&&( !strcasecmp( cur->name, name ))){
      free( cur->name );
      free( cur->value );
      free( cur->domain ); 
      cur->name  = strdup( name );
      cur->value = strdup( value );
      if( !d )
        cur->domain = strdup( host );
      else
        cur->domain = strdup( domain ); 
      found = TRUE;
      break;
    }
  }
  if( !found ){
    new = (CNODE*)malloc( sizeof( CNODE ));
    if( !new ) joe_fatal( "out of memory!" ); 
    new->threadID = id;
    new->name     = strdup( name );
    new->value    = strdup( value );
    if( !d )
      new->domain = strdup( host );
    else
      new->domain = strdup( domain );
    new->next = cur;
    if( cur==cookie->first )
      cookie->first = new;
    else
      pre->next = new;    
  }
  if( name  != NULL ) free( name );
  if( value != NULL ) free( value );

  pthread_mutex_unlock(&(cookie->mutex));

  return 0;
}

int
delete_cookie( int id, char *name )
{
  CNODE  *cur, *pre;
  
  printf( "before lock\n" );
  pthread_mutex_lock(&(cookie->mutex));
  printf( "inside delete\n" );
  for( cur=pre=cookie->first; cur != NULL; pre=cur, cur=cur->next ){
    if( cur->threadID == id ){
      if( !strcasecmp( cur->name, name )){
        pre->next = cur->next;
        free(cur);
        if( my.debug ){printf("Cookie deleted: %d => %s\n",id,name); fflush(stdout);}
        break;
      }
    } 
    else{
      continue; 
    }
  }
  pthread_mutex_unlock(&(cookie->mutex));
  return 0;
}

/* 
  get_cookie returns a char * with a compete 
  value of the Cookie: string.  It does NOT
  return the actual Cookie struct.      
*/
char *
get_cookie( int id, char *host, char *newton )
{
  char   oreo[MAX_COOKIE_SIZE];  
  int    dlen, hlen; 
  CNODE  *pre, *cur;

  memset( oreo, '\0', sizeof( oreo ));
  hlen = strlen( host ); 

  pthread_mutex_lock(&(cookie->mutex));
 
  for( cur=pre=cookie->first; cur != NULL; pre=cur, cur=cur->next ){
    dlen = cur->domain ? strlen( cur->domain ) : 0;
    if( cur->threadID == id ){
      if( !strcasecmp( cur->domain, host )){
        if( strlen( oreo ) > 0 )
          strncat( oreo, ";",        sizeof( oreo ));
          strncat( oreo, cur->name,  sizeof( oreo ));
          strncat( oreo, "=",        sizeof( oreo ));
          strncat( oreo, cur->value, strlen(cur->value));
      }
      if(( dlen < hlen ) && ( !strcasecmp( host + ( hlen - dlen ), cur->domain ))){
        if( strlen( oreo ) > 0 )
          strncat( oreo, ";",        sizeof( oreo ));
          strncat( oreo, cur->name,  sizeof( oreo ));
          strncat( oreo, "=",        sizeof( oreo ));
          strncat( oreo, cur->value, strlen(cur->value));
      } 
    }
  }
  strncpy( newton, oreo, sizeof( oreo ));
  pthread_mutex_unlock(&(cookie->mutex));

  return newton;
}

/*
  Helper util, displays the contents of Cookie
*/
void
display_cookies()
{
  CNODE *cur;
 
  pthread_mutex_lock(&(cookie->mutex));
 
  printf ("Linked list contains:\n");
  for( cur=cookie->first; cur != NULL; cur=cur->next ) {
    printf ("Index: %d\tName: %s Value: %s\n", cur->threadID, cur->name, cur->value);
  }
 
  pthread_mutex_unlock(&(cookie->mutex));
 
  return;
}

