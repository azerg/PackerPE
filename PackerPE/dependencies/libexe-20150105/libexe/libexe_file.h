/*
 * File functions
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

#if !defined( _LIBEXE_INTERNAL_FILE_H )
#define _LIBEXE_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libexe_extern.h"
#include "libexe_io_handle.h"
#include "libexe_libbfio.h"
#include "libexe_libcdata.h"
#include "libexe_libcerror.h"
#include "libexe_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_internal_file libexe_internal_file_t;

struct libexe_internal_file
{
	/* The IO handle
	 */
	libexe_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The sections array
	 */
	libcdata_array_t *sections_array;
};

LIBEXE_EXTERN \
int libexe_file_initialize(
     libexe_file_t **file,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_free(
     libexe_file_t **file,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_signal_abort(
     libexe_file_t *file,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_open(
     libexe_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBEXE_EXTERN \
int libexe_file_open_wide(
     libexe_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBEXE_EXTERN \
int libexe_file_open_file_io_handle(
     libexe_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_close(
     libexe_file_t *file,
     libcerror_error_t **error );

int libexe_file_open_read(
     libexe_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_get_ascii_codepage(
     libexe_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_set_ascii_codepage(
     libexe_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error );

int libexe_file_get_offset_by_relative_virtual_address(
     libexe_internal_file_t *internal_file,
     uint32_t virtual_address,
     off64_t *offset,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_get_number_of_sections(
     libexe_file_t *file,
     int *number_of_sections,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_get_section(
     libexe_file_t *file,
     int section_index,
     libexe_section_t **section,
     libcerror_error_t **error );

LIBEXE_EXTERN \
int libexe_file_get_section_by_name(
     libexe_file_t *file,
     const char *name,
     size_t name_length,
     libexe_section_t **section,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

