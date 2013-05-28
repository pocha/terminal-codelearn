/**
 * Configuration file support
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

/** 
 * Ignores comment lines beginning with 
 * '#' empty lines beginning with \n 
 * Takes a char* as an argument        
 */
void 
parse(char *str)
{
  char *ch;
  ch = (char *)strstr(str, "#"); 
  if ( ch ){ *ch = 0; }
  ch = (char *)strstr(str, "\n"); 
  if ( ch ){ *ch = 0; }
}

/** 
 * Reads filename into memory and populates
 * the config_t struct with the result. Uses
 * parse to ignore comments and empty lines. 
 */ 
int 
read_cfg_file( LINES *l, char *filename )
{
  /* file pointer  */
  FILE *file; 

  /* char array to hold contents */
  char head[1024]; 
  
  /* make sure config_t has been initialized. */
  if( !l ){	
    printf( "Structure not initialized!\n" );
    return -1;
  }

  if(( file = fopen(filename, "r")) == NULL ) {
    /* this is a fatal problem, but we want  
       to enlighten the user before dying   */
    display_help();
    exit( EXIT_FAILURE );
  }

  l->index = 0;
  while( fgets( head, sizeof( head ), file ) != NULL ){
    parse( head );
    chomp( head );
    if( strlen( head ) == 0 );
    else{
      l->line = (void *)realloc(l->line, sizeof(char *) * (l->index + 1));
      l->line[l->index] = (char *)strdup( head );
      l->index++;	
    }
  }
  fclose( file );
  return l->index;
}

