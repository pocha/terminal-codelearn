/**
 * Load Post Data
 *
 * Copyright (C) 2002 by
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
#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <load.h>
#include <errno.h>
#include <joedog/joedog.h>

char *
load_file( char *file, char *buf )
{
  FILE *fp;
  char tmp[POST_BUF];
  char *filename;
  struct stat st; 

  filename = tchomp( file );

  if( lstat( filename, &st ) < 0 ){
    errno = -5;
    joe_error( "unable to get statistics for: %s", filename );
  }

  if( st.st_size > POST_BUF ){
    joe_warning( "POST file too large: %s", filename );
  }

  if(( fp = fopen( filename, "r" )) == NULL ) {
    joe_error( "could not open file: %s", filename );
  }

  fgets( tmp, POST_BUF, fp );
 
  fclose( fp );  

  strncpy( buf, tmp, sizeof( tmp ));
  return( buf );
}

