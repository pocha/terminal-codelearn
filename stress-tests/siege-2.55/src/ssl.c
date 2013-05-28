/**
 * SSL Thread Safe Setup Functions.
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
 */
#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <ssl.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

/**
 * these functions were more or less taken from
 * the openssl thread safe examples included in
 * the OpenSSL distribution.
 */

#ifdef HAVE_SSL
void 
SSL_thread_setup( void ) 
{
  int x;
 
#define OPENSSL_THREAD_DEFINES
#include <openssl/opensslconf.h>
#if defined(THREADS)
#else
   fprintf(stderr, "WARNING: your openssl libraries were compiled without thread support\n");
   pthread_sleep_np( 2 );
#endif
 
  lock_cs    = (pthread_mutex_t*)OPENSSL_malloc(CRYPTO_num_locks()*sizeof(pthread_mutex_t));
  lock_count = (long*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));

  for( x = 0; x < CRYPTO_num_locks(); x++ ){
    lock_count[x] = 0;
    pthread_mutex_init(&(lock_cs[x]), NULL);
  }
  CRYPTO_set_id_callback((unsigned long (*)())SSL_pthreads_thread_id);
  CRYPTO_set_locking_callback((void (*)())SSL_pthreads_locking_callback);
}

void 
SSL_thread_cleanup( void ) 
{
  int x;
  
  CRYPTO_set_locking_callback(NULL);
  for( x = 0; x < CRYPTO_num_locks(); x++ ) {
    pthread_mutex_destroy(&(lock_cs[x]));
  }
  OPENSSL_free(lock_cs);
  OPENSSL_free(lock_count);
}

void 
SSL_pthreads_locking_callback(int mode, int type, char *file, int line) 
{
  if(mode & CRYPTO_LOCK){
    pthread_mutex_lock(&(lock_cs[type]));
    lock_count[type]++;
  } 
  else{ 
    pthread_mutex_unlock(&(lock_cs[type]));
  }
}

unsigned long 
SSL_pthreads_thread_id(void) 
{
  unsigned long ret;
  ret = (unsigned long)pthread_self();

  return(ret);
}

#endif/*HAVE_SSL*/
