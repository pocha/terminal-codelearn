/* include/config.h.  Generated automatically by configure.  */
/* include/config.h.in.  Generated automatically from configure.in by autoheader.  */
/* Name of this package     */
#define PACKAGE "siege"

/* Version number           */
#define VERSION "2.55"

/* Define debugging         */
/* #undef DEBUG */

/* Define OS                */
/* #undef OS */

/* Define sige-system type  */
/* #undef SIEGE_SYSTEM_TYPE */

/* Define ssl support       */
/* #undef HAVE_SSL */

/* Define setpgrp void      */
/* #undef SETPGRP_VOID */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix). */
#define HAVE_ALLOCA_H 1

/* Define if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define if you have the `memcpy' function. */
#define HAVE_MEMCPY 1

/* Define if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have the `strlen' function. */
#define HAVE_STRLEN 1

/* Define if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define if you have the `strncmp' function. */
#define HAVE_STRNCMP 1

/* Define if you have the `strncpy' function. */
#define HAVE_STRNCPY 1

/* Define if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "siege"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Version number of package */
#define VERSION "2.55"

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* # undef _ALL_SOURCE */
#endif

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */
