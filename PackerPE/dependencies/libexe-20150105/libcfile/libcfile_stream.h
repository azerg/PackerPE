/*
 * File stream functions
 *
 * Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBCFILE_INTERNAL_STREAM_H )
#define _LIBCFILE_INTERNAL_STREAM_H

#include <common.h>
#include <types.h>

#if defined( HAVE_GLIB_H )
#include <glib.h>
#include <glib/gstdio.h>
#endif

#include <stdio.h>

#include "libcfile_extern.h"
#include "libcfile_libcerror.h"
#include "libcfile_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcfile_internal_stream libcfile_internal_stream_t;

struct libcfile_internal_stream
{
	FILE *stream;
};

LIBCFILE_EXTERN \
int libcfile_stream_initialize(
     libcfile_stream_t **stream,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_stream_free(
     libcfile_stream_t **stream,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_stream_open(
     libcfile_stream_t *stream,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

/* TODO */
#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBCFILE_EXTERN \
int libcfile_stream_open_wide(
     libcfile_stream_t *stream,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );
#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBCFILE_EXTERN \
int libcfile_stream_close(
     libcfile_stream_t *stream,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_stream_read_buffer(
         libcfile_stream_t *stream,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_stream_write_buffer(
         libcfile_stream_t *stream,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
off64_t libcfile_stream_seek_offset(
         libcfile_stream_t *stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_stream_get_offset(
     libcfile_stream_t *stream,
     off64_t *offset,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_stream_get_size(
     libcfile_stream_t *stream,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

