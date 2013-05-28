/**
 * Utility functions
 * Library: joedog
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
#include <util.h>


/**
 * substring        returns a char pointer from
 *                  start to start + length in a
 *                  larger char pointer (buffer).
 */
char * 
substring (char *buffer, int start, int length)
{
  char *sub;
  sub = malloc (sizeof (char) * (length + 1));
	
  if ((length < 1) || (start < 0) || (start > strlen (buffer)))
    return NULL;

  if (strlen (buffer) < length){
    sub = (char*) strdup (buffer);
    return buffer;
  }

  if (sub == NULL){
    joe_fatal( "insufficient memory." );
  }

  memset (sub, '\0', length + 1);

  buffer += start;
  strncpy( sub, buffer, length );

  return sub;
}

/**
 * my_random        returns a random int
 */  
int 
my_random( int max, int seed )
{
  srand( (unsigned)time( NULL ) * seed ); 
  return (int)((double)rand() / ((double)RAND_MAX + 1) * max ); 
}

void 
itoa( int n, char s[] )
{
  int i, sign;
  if(( sign = n ) < 0 )
    n = -n;
  i = 0;
  do{
    s[i++] = n % 10 + '0';
  } while(( n /= 10 ) > 0 );
  if( sign < 0  )
    s[i++] = '-';
  s[i] = '\0';
 
  reverse( s );
}

void 
reverse( char s[] )
{
  int c, i, j;
  for( i = 0, j = strlen( s )-1; i < j; i++, j-- ){
    c    = s[i];
    s[i] = s[j];
    s[j] = c;
  } /** end of for     **/
}   /** end of reverse **/

float 
elapsed_time( clock_t time )
{
  long tps = sysconf( _SC_CLK_TCK );
  return (float)time/tps;
}

