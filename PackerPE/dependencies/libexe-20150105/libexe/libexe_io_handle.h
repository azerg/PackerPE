/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBEXE_IO_HANDLE_H )
#define _LIBEXE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libexe_data_directory_descriptor.h"
#include "libexe_definitions.h"
#include "libexe_libbfio.h"
#include "libexe_libcdata.h"
#include "libexe_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *exe_mz_signature;

typedef struct libexe_io_handle libexe_io_handle_t;

struct libexe_io_handle
{
	/* The executable type
	 */
	uint8_t executable_type;

	/* The creation time
	 */
	uint32_t creation_time;

	/* The data directory
	 */
        libexe_data_directory_descriptor_t data_directories[ LIBEXE_NUMBER_OF_DATA_DIRECTORIES ];

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libexe_io_handle_initialize(
     libexe_io_handle_t **io_handle,
     libcerror_error_t **error );

int libexe_io_handle_free(
     libexe_io_handle_t **io_handle,
     libcerror_error_t **error );

int libexe_io_handle_clear(
     libexe_io_handle_t *io_handle,
     libcerror_error_t **error );

int libexe_io_handle_read_file_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_mz_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *extended_header_offset,
     libcerror_error_t **error );

int libexe_io_handle_read_extended_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t extended_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_le_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t le_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_ne_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t ne_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_pe_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t pe_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_coff_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_sections,
     libcerror_error_t **error );

int libexe_io_handle_read_coff_optional_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t optional_header_size,
     libcerror_error_t **error );

int libexe_io_handle_read_section_table(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t number_of_sections,
     libcdata_array_t *sections_array,
     libcerror_error_t **error );

ssize_t libexe_io_handle_read_segment_data(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libexe_io_handle_seek_segment_offset(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

