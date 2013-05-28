#ifndef JOEDOG_H
#define JOEDOG_H 
/**
 * JOEDOG HEADER 
 *
 * Copyright (C) 2000 Jeffrey Fulmer <jdfulmer@armstrong.com>
 * This file is part of Siege
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
 */
#include <config.h>
#include <time.h>  
#include <stdarg.h>

#define TRUE  1
#define FALSE 0

/**
 * Error handling 
 */
extern void joe_warning( const char *message, ... );

extern void joe_error( const char *message, ... );

extern void joe_fatal( const char *message, ... );

/**
 * Memory management
 */
#define SIEGEmalloc(x) joe_malloc( x, __FILE__, __LINE__)

void * xrealloc( void *, size_t );
 
void * xmalloc ( size_t );
 
void * xcalloc ( size_t, size_t ); 

/**
 * Utility functions
 */
void itoa( int, char [] );
 
void reverse( char [] );
 
int  my_random( int, int );
 
char * substring( char *, int, int );
 
float elapsed_time( clock_t );

/**
 * snprintf functions
 */
#define PORTABLE_SNPRINTF_VERSION_MAJOR 2
#define PORTABLE_SNPRINTF_VERSION_MINOR 2

#ifdef HAVE_SNPRINTF
#include <stdio.h>
#else
extern int snprintf(char *, size_t, const char *, /*args*/ ...);
extern int vsnprintf(char *, size_t, const char *, va_list);
#endif

#if defined(HAVE_SNPRINTF) && defined(PREFER_PORTABLE_SNPRINTF)
extern int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
extern int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
#define snprintf  portable_snprintf
#define vsnprintf portable_vsnprintf
#endif

extern int asprintf  (char **ptr, const char *fmt, /*args*/ ...);
extern int vasprintf (char **ptr, const char *fmt, va_list ap);
extern int asnprintf (char **ptr, size_t str_m, const char *fmt, /*args*/ ...);
extern int vasnprintf(char **ptr, size_t str_m, const char *fmt, va_list ap);

/**
 * not quite perl chomp, this function hacks the 
 * white space off the end of string str.
 */
char *chomp( char *str );

/** 
 * left chomp, removes the leading whitespace from 
 * string str. 
 */
char *lchomp( char *str );

/**
 * total chomp, removes the white space from the 
 * beginning and the end of string str.
 */
char *tchomp( char *str );

/**
 * split s by pattern and return **
 */
char **split( char pattern, char *s, int *n_words ); 

#endif  /* JOEDOG_H */  




