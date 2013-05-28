#ifdef HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <stdio.h>
#include <memory.h>
#include <perl.h>

#ifdef STDC_HEADERS
# include <string.h>
#else
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
char *strchr (), *strrchr ();
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif

/**
 * local prototypes
 */
int word_count( char pattern, char *s ); 

/**
 * not quite perl chomp, this function
 * hacks the white space off the end of
 * string str.
 */
char *
chomp( char *str )
{
  char *ch, *p;

  ch = p = str;
  while( *ch && *ch <= ' ' ) ch++;

  /* assign it */
  while( *p++ = *ch++ );

  /* our work is done. */
  return str;
}


/** 
 * left chomp, removes the leading 
 * whitespace from string str. 
 */
char * 
lchomp( char *str ){
   char *s, *p;
   for (s = p = str; *s && *s <= ' '; s++) ;
   while ((*p++ = *s++)) ;
   return str;
}

/**
 * total chomp, removes the white
 * space from the beginning and the
 * end of string str.
 */
char * 
tchomp( char *str )
{
  return chomp(lchomp( str ));
}

/**
 * local function used by split,
 * returns int words in string.
 */
int
word_count( char pattern, char *s )
{
  int in_word_flag = 0;
  int count = 0;
  char *ptr;
 
  ptr = s;
  while( *ptr ){
    if(( *ptr ) != pattern ){
      if( in_word_flag == 0 )
        count++;
      in_word_flag = 1;
    }
    else
      in_word_flag = 0;
    ptr++;
  }
  return count;
} 

char **
split( char pattern, char *s, int *n_words )
{
  char **words = NULL;
  char *str0, *str1;
  int i;
  *n_words = word_count( pattern, s);
 
  words = xmalloc( *n_words * sizeof (*words));
  if( !words )
    return NULL;

  if( s[0] == pattern ) *s++; 
  str0 = s;
  i = 0;
  while( *str0 ){
    size_t len;
    str1 = strchr( str0, pattern );
    if( str1 != NULL )
      len = str1 - str0;
    else
      len = strlen( str0 );
 
    words[i] = xmalloc( len + 1 );
    if( !words[i] ){
      fprintf(
        stderr,
        "Warning: You got %d words out of %d.",
        i + 1, *n_words
      );
      return words;
    }
    strncpy( words[i], str0, len );
    words[i][len] = '\0';
    
   if( str1 != NULL )
      str0 = ++str1;
    else
      break;
    i++;
  }
  return words;
} 


