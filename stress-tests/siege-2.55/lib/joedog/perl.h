#ifndef PERL_H
#define PERL_H

/**
 * not quite perl chomp, this function
 * hacks the white space off the end of
 * string str.
 */
char *chomp( char *str );


/** 
 * left chomp, removes the leading 
 * whitespace from string str. 
 */
char *lchomp( char *str );

/**
 * total chomp, removes the white
 * space from the beginning and the
 * end of string str.
 */
char *tchomp( char *str );

/**
 * split s by pattern and return **
 */
char **split( char pattern, char *s, int *n_words );

#endif
