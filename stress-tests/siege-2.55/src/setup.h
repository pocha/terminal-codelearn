/**
 * Package header
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
#ifndef SETUP_H
#define SETUP_H

#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <ctype.h>
#include <stdarg.h>

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif

#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#ifdef  HAVE_UNISTD_H
# include <unistd.h>
#endif/*HAVE_UNISTD_H*/

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

#ifdef  HAVE_SYS_TIMES_H
# include <sys/times.h>
#endif/*HAVE_SYS_TIMES_H*/

#if  TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif/*TIME_WITH_SYS_TIME*/

#if HAVE_ERRNO_H
# include <errno.h>
#endif /* HAVE_ERRNO_H */

#ifndef HAVE_SNPRINTF
# define portable_snprintf  snprintf
# define portable_vsnprintf vsnprintf
#endif

#ifndef  EXIT_SUCCESS
# define EXIT_SUCCESS   0
#endif /*EXIT_SUCESS*/
#ifndef  EXIT_FAILURE
# define EXIT_FAILURE   1
#endif /*EXIT_FAILURE*/

#ifndef  PTHREAD_CREATE_JOINABLE
# define PTHREAD_CREATE_JOINABLE  0
#endif /*PTHREAD_CREATE_JOINABLE*/ 
#ifndef  PTHREAD_CREATE_DETACHED
# define PTHREAD_CREATE_DETACHED  1
#endif /*PTHREAD_CREATE_DETACHED*/


#ifndef HAVE_STRCASECMP
int strcasecmp();
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp();
#endif
#ifndef HAVE_STRNCMP
int strncmp();
#endif
#ifndef HAVE_STRLEN
int strlen();
#endif
#include <joedog/joedog.h>
#include <joedog/joepath.h>

#define  TRUE           1
#define  FALSE	        0
#define  MXCHLD         1024
#define  MSG_NOERROR    010000
#define  ISSEPARATOR(x) ('='==(x))
#define  ISASCII(x)     isascii ((unsigned char)(x))
#define  ISALPHA(x)     isalpha ((unsigned char)(x))
#define  ISSPACE(x)     isspace ((unsigned char)(x))
#define  ISDIGIT(x)     isdigit ((unsigned char)(x)) 
#if STDC_HEADERS
# define TOLOWER(Ch) tolower (Ch)
# define TOUPPER(Ch) toupper (Ch)
#else
# define TOLOWER(Ch) (ISUPPER (Ch) ? tolower (Ch) : (Ch))
# define TOUPPER(Ch) (ISLOWER (Ch) ? toupper (Ch) : (Ch))
#endif

#define F_CONNECTING  1
#define F_READING     2
#define F_DONE        4

#define MAXREPS       10301062

void display_version( int i );
void display_help();

/** 
 * struct configuration 
 */
struct CONFIG
{
  int  logging;      /* boolean, log transactions to log file   */
  int  shlog;        /* show log file configuration directive.  */
  char logfile[128]; /* alternative user defined simbot.log     */ 
  int  verbose;      /* boolean, verbose output to screen       */
  int  config;       /* boolean, prints the configuration       */
  int  cusers;       /* default concurrent users value.         */
  int  delay;        /* range for random time delay, see -d     */
  int  timeout;      /* socket connection timeout value, def:10 */
  int  bench;        /* signifies a benchmarking run, no delay  */
  int  internet;     /* boolean, tell siege to use random mode. */
  int  time;         /* length of the siege in hrs, mins, secs  */
  int  secs;         /* time value for the lenght of the siege  */
  int  reps;         /* reps to run the test, default infinite  */ 
  char file[128];    /* urls.txt file, default in joepath.h     */
  int  length;       /* length of the urls array, made global   */
  int  debug;        /* boolean, undocumented debug command     */
  int  mark;         /* boolean, signifies a log file mark req. */ 
  char *markstr;     /* user defined string value to mark file  */
  char *url;         /* URL for the single hit invocation.      */
  int  protocol;     /* 0=HTTP/1.0; 1=HTTP/1.1                  */
  int  cookies;      /* boolean, to use cookies or not.         */
  char uagent[256];  /* user defined User-Agent string.         */
  char *username;    /* WWW-Authenticate user name              */
  char *password;    /* WWW-Authenticate password.              */
  char *auth;        /* WWW-Authenticate base64 usr:pas holder  */
  int  authorize;    /* WWW-Authenticate boolean, require auth  */
  int  keepalive;    /* boolean, connection keep-alive value    */
  int  signaled;     /* timed based testing notification bool.  */
  char extra[512];   /* extra http request headers              */ 
  int  proxy;        /* boolean, TRUE == use a proxy server.    */
  char *proxysrv;    /* hostname for the proxy server.          */
  int  proxyport;    /* port number for proxysrv                */ 
  int  login;        /* boolean, client must login first.       */
  char *loginurl;    /* the initial login URL, first URL hit.   */
  int  failures;     /* number of failed attempts before abort. */
  int  failed;       /* total number of socket failures.        */
  int  follow;       /* boolean, TRUE == follow 302             */
  int  zero_ok;      /* boolean, TRUE == zero bytes data is OK. */ 
};

struct STATUS
{
  int             total_clients;  /* high water thread mark   */
  int             total_threads;  /* number of active threads */
  pthread_cond_t  exit_cond;      /* pthread cond. variable   */
  pthread_mutex_t mutex_lock;     /* w lock for total_threads */
  int             shutting_down;  /* boolean: 1=TRUE, 0=FALSE */
};

/**
 * struct LINES, read by read_cfg_file
 * in config.c 
 */
typedef struct
{
  int  index;
  char **line;
} LINES;

/**
 * enum PROTOCOL
 * tcp/ip protocol
 */
typedef enum
{
  HTTP        = 0,
  HTTPS       = 1,
  UNSUPPORTED = 2
} PROTOCOL;

/**
 * enum DIRECTIVE
 * HTTP request directive
 */
typedef enum
{
  URL_GET   = 0,
  URL_POST  = 1
} DIRECTIVE;

/**
 * URL struct URL
 */ 
typedef struct
{
  PROTOCOL protocol;       /* currently only http/https   */
  char *hostname;          /* DNS entry or IP address     */
  int  port;               /* tcp port num, defs: 80, 443 */
  char *pathname;          /* path to http resource.      */
  DIRECTIVE calltype;      /* request: GET/POST/HEAD etc. */
  size_t postlen;
  char *postdata;
  char *posttemp;
} URL;

/**
 * CONNection information
 */
typedef struct
{
  int    hits;
  int    bytes;
  int    code;
  int    fail;
  float  time;
  int    status;
} DATA;  


/**
 * cookie node
 */
typedef struct CNODE
{
  int    index;
  int    threadID;
  char   *name;
  char   *value;
  char   *path;
  char   *domain;
  time_t expires;
  char   *expirestr;
  char   *version;
  char   *max;
  int    secure;
  struct CNODE  *next;
} CNODE;

typedef struct
{
  CNODE *first;
  pthread_mutex_t mutex; 
} COOKIE;


#if INTERN
# define EXTERN /* */
#else
# define EXTERN extern
#endif
 
EXTERN struct CONFIG my;
EXTERN struct STATUS our;
EXTERN URL  *urls;
EXTERN char *version_string; 
EXTERN COOKIE *cookie; 

#endif  /* SETUP_H */
