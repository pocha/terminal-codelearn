/**
 * Transacation logging support
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
#include <setup.h>
#include <fcntl.h>
#include <log.h>
#include <locale.h>

/**
 * writes the output from siege to a formatted
 * log file.  checks if the log exists, if not
 * it creates a new formatted file and appends
 * text to it.  If a file does exist,  then it
 * simply appends to it. 
 */
void
write_to_log( int count, float elapsed, int bytes, float ttime, int code, int failed )
{
  /* file descriptor handle   */
  int  fd; 

  /* invoke with localtime    */
  time_t  clock;   
  struct tm *tm;  

  /* holds the log file entry */
  char log_entry[512];

  /* char array date entry    */
  char date[100];

  /* set the local time       */ 
  time( &clock );
  tm = localtime( &clock );
  setlocale( LC_TIME, "C" );
  strftime( date, sizeof( date ), "%x %X", tm ); 

  /* if the file does NOT exist then we'll create it.    */
  if( my.shlog ){ 
    printf( "FILE: %s\n", my.logfile ); 
    puts( "You can disable this annoying message by editing" );
    puts( "the .siegerc file in your home directory; change" );
    puts( "the directive \'show-logfile\' to false." );
  }
  if(( file_exists( my.logfile )) < 0 ){
    if(( create_logfile( my.logfile )) < 0 ){
      joe_error( "unable to create log file, do you have permission?" );
      return;
    }
  }

  /* create the log file entry with function params. */
  sprintf( log_entry, "%s,%7d,%11.2f,%12u,%11.2f,%12.2f,%12.2f,%12.2f,%8d,%8d\n", date, count, elapsed, bytes, ttime / count, count / elapsed, bytes / elapsed, ttime / elapsed, code, failed );

  /* open the log and write to file */
  if((fd = open( my.logfile, O_WRONLY | O_APPEND, 0644 )) < 0 ) {
    joe_error( "Unable to write to file" );
    /** not necessarily fatal **/
  }
  write( fd, log_entry, strlen( log_entry ));
  close( fd );

 
  return;
}  

/* marks the siege.log with a user defined 
   message.  checks for the existence of a
   log and creates one if not found.      */
void
mark_log_file( char *message )
{
  /* file descriptor handle   */
  int  fd;

  /* char array log entry     */
  char log_entry[512];

  /* if the file does NOT exist then create it.  */
  if(( file_exists( my.logfile )) < 0 ){
    if(( create_logfile( my.logfile )) < 0 ){
      joe_error( "unable to create log file, do you have permission?" );
      return;
    }
  }

  /* create the log file entry */
  snprintf( log_entry, sizeof( log_entry ), "**** %s ****\n", message );

  /* open the file and write to it */
  if((fd = open( my.logfile, O_WRONLY | O_APPEND, 0644 )) < 0 ) {
    joe_error( "Unable to write to file" );
    /** not necessarily fatal **/
  }
  write( fd, log_entry, strlen( log_entry ));
  close( fd );

  return; 

}
/**
 * returns 0 if the file exists,
 * -1 if it does not.
 */
int
file_exists( char *file )
{
  /* file descriptor handle   */
  int  fd;

  /* open the file read only  */
  if(( fd = open( file, O_RDONLY )) < 0 ){
  /* the file does NOT exist  */
    close( fd );
    return -1;
  }
  else{
  /* party on Garth... */
    close( fd );
    return 0;
  }

  return -1;
}

/**
 * return 0 upon the successful
 * completion of the file, -1 if not. The
 * function adds a header at the top of the
 * file, format is comma separated text for
 * spreadsheet import.
 */
int
create_logfile( const char *file )
{
  /* file descriptor handle   */
  int   fd;
  
  /* log file desc. header    */
  char *head = "      Date & Time,  Trans,  Elap Time,  Data Trans,  Resp Time,  Trans Rate,  Throughput,  Concurrent,    OKAY,   Failed\n"; 
 
  /* open the file write only */ 
  if(( fd = open( file, O_CREAT | O_WRONLY, 0644 )) < 0 ){
    return -1;
  }

  /* write the header to the file */
  write( fd, head, strlen( head ));
  close( fd );
  
  return 0;
} 

