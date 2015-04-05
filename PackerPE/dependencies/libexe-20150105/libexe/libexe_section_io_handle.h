/*
 * Section IO handle functions
 *
 * Copyright (C) 2011-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEXE_SECTION_IO_HANDLE_H )
#define _LIBEXE_SECTION_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libexe_libcerror.h"
#include "libexe_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_section_io_handle libexe_section_io_handle_t;

struct libexe_section_io_handle
{
	/* The section
	 */
	libexe_section_t *section;

	/* Value to indicate the IO handle is open
	 */
	uint8_t is_open;

	/* The current access flags
	 */
	int access_flags;
};

int libexe_section_io_handle_initialize(
     libexe_section_io_handle_t **io_handle,
     libexe_section_t *section,
     libcerror_error_t **error );

int libexe_section_io_handle_free(
     libexe_section_io_handle_t **io_handle,
     libcerror_error_t **error );

int libexe_section_io_handle_clone(
     libexe_section_io_handle_t **destination_io_handle,
     libexe_section_io_handle_t *source_io_handle,
     libcerror_error_t **error );

int libexe_section_io_handle_open(
     libexe_section_io_handle_t *io_handle,
     int flags,
     libcerror_error_t **error );

int libexe_section_io_handle_close(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error );

ssize_t libexe_section_io_handle_read(
         libexe_section_io_handle_t *io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

ssize_t libexe_section_io_handle_write(
         libexe_section_io_handle_t *io_handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t libexe_section_io_handle_seek_offset(
         libexe_section_io_handle_t *io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int libexe_section_io_handle_exists(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error );

int libexe_section_io_handle_is_open(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error );

int libexe_section_io_handle_get_size(
     libexe_section_io_handle_t *io_handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

