/*
 * Section functions
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

#if !defined( _LIBEXE_INTERNAL_SECTION_H )
#define _LIBEXE_INTERNAL_SECTION_H

#include <common.h>
#include <types.h>

#include "libexe_extern.h"
#include "libexe_io_handle.h"
#include "libexe_libbfio.h"
#include "libexe_libcerror.h"
#include "libexe_section_descriptor.h"
#include "libexe_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_internal_section libexe_internal_section_t;

struct libexe_internal_section
{
	/* The IO handle
	 */
	libexe_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The section descriptor
	 */
	libexe_section_descriptor_t *section_descriptor;
};

int libexe_section_initialize(
     libexe_section_t **section,
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libexe_section_descriptor_t *section_descriptor,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_free(
     libexe_section_t **section,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_name_size(
     libexe_section_t *section,
     size_t *name_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_name(
     libexe_section_t *section,
     char *name,
     size_t name_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_utf8_name_size(
     libexe_section_t *section,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_utf8_name(
     libexe_section_t *section,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_utf16_name_size(
     libexe_section_t *section,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_utf16_name(
     libexe_section_t *section,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
ssize_t libexe_section_read_buffer(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBEXE_EXTERN \
ssize_t libexe_section_read_buffer_at_offset(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBEXE_EXTERN \
off64_t libexe_section_seek_offset(
         libexe_section_t *section,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_offset(
     libexe_section_t *section,
     off64_t *offset,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_size(
     libexe_section_t *section,
     size64_t *size,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_start_offset(
     libexe_section_t *section,
     off64_t *start_offset,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_virtual_address(
     libexe_section_t *section,
     uint32_t *virtual_address,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_section_get_data_file_io_handle(
     libexe_section_t *section,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

