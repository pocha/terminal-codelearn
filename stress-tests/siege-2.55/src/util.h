/**
 * Utility Functions
 *
 * Copyright (C) 2001 Jeffrey Fulmer <jdfulmer@armstrong.com>
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
 *
 */
#ifndef  UTIL_H
#define  UTIL_H

void parse_time( char *p );

void pthread_sleep_np( int seconds ); 
void pthread_usleep_np( unsigned long usec ); 

#endif /*UTIL_H*/
