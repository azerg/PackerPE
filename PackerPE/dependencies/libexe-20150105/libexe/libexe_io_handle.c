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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libexe_codepage.h"
#include "libexe_data_directory_descriptor.h"
#include "libexe_definitions.h"
#include "libexe_io_handle.h"
#include "libexe_libbfio.h"
#include "libexe_libcerror.h"
#include "libexe_libcdata.h"
#include "libexe_libcnotify.h"
#include "libexe_libcstring.h"
#include "libexe_libfdatetime.h"
#include "libexe_section_descriptor.h"
#include "libexe_unused.h"

#include "exe_file_header.h"
#include "exe_section_table.h"

const char *exe_mz_signature = "MZ";
const char *exe_le_signature = "LE";
const char *exe_ne_signature = "NE";
const char *exe_pe_signature = "PE\x0\x0";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_initialize(
     libexe_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libexe_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libexe_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 *io_handle );

		*io_handle = NULL;

		return( -1 );
	}
	( *io_handle )->executable_type = LIBEXE_EXECUTABLE_TYPE_MZ;
	( *io_handle )->ascii_codepage  = LIBEXE_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_free(
     libexe_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libexe_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_clear(
     libexe_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libexe_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->executable_type = LIBEXE_EXECUTABLE_TYPE_MZ;
	io_handle->ascii_codepage  = LIBEXE_CODEPAGE_WINDOWS_1252;

	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_file_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	static char *function           = "libexe_io_handle_read_file_header";
	uint32_t extended_header_offset = 0;

	if( libexe_io_handle_read_mz_header(
	     io_handle,
	     file_io_handle,
	     &extended_header_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MZ header.",
		 function );

		return( -1 );
	}
/* TODO check if value is sane */
	if( extended_header_offset != 0 )
	{
/* TODO print data between current offset and extended_header_offset */

		if( libexe_io_handle_read_extended_header(
		     io_handle,
		     file_io_handle,
		     extended_header_offset,
		     number_of_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extended header.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the MZ header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_mz_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *extended_header_offset,
     libcerror_error_t **error )
{
	exe_mz_header_t mz_header;

	static char *function                 = "libexe_io_handle_read_mz_header";
	ssize_t read_count                    = 0;
	uint16_t number_of_relocation_entries = 0;
	uint16_t relocation_table_offset      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                  = 0;
	uint16_t value_16bit                  = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( extended_header_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended header offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading MZ header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek MZ header offset: 0.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &mz_header,
	              sizeof( exe_mz_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( exe_mz_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MZ header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MZ header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &mz_header,
		 sizeof( exe_mz_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     mz_header.signature,
	     exe_mz_signature,
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 mz_header.number_of_relocation_entries,
	 number_of_relocation_entries );

	byte_stream_copy_to_uint16_little_endian(
	 mz_header.relocation_table_offset,
	 relocation_table_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 mz_header.signature[ 0 ],
		 mz_header.signature[ 1 ] );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.last_page_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: last page size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.number_of_pages,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of pages\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of relocation entries\t\t: %" PRIu16 "\n",
		 function,
		 number_of_relocation_entries );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.number_of_header_paragraphs,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of header paragraphs\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.minimum_allocated_paragraphs,
		 value_16bit );
		libcnotify_printf(
		 "%s: minimum allocated paragraphs\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.maximum_allocated_paragraphs,
		 value_16bit );
		libcnotify_printf(
		 "%s: maximum allocated paragraphs\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.initial_stack_segment,
		 value_16bit );
		libcnotify_printf(
		 "%s: initial stack segment\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.initial_stack_pointer,
		 value_16bit );
		libcnotify_printf(
		 "%s: initial stack pointer\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.checksum,
		 value_16bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 mz_header.entry_point,
		 value_32bit );
		libcnotify_printf(
		 "%s: entry point\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: relocation table offset\t\t: 0x%04" PRIx16 "\n",
		 function,
		 relocation_table_offset );

		byte_stream_copy_to_uint16_little_endian(
		 mz_header.overlay_number,
		 value_16bit );
		libcnotify_printf(
		 "%s: overlay number\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( relocation_table_offset >= 0x40 )
	{
/* TODO read data */
		byte_stream_copy_to_uint32_little_endian(
		 mz_header.extended_header_offset,
		 *extended_header_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 mz_header.unknown1,
			 32,
			 0 );

			libcnotify_printf(
			 "%s: extended header offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 *extended_header_offset );

			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 mz_header.unknown2,
			 112,
			 0 );
		}
#endif
	}
/* TODO print data between realloc and current offset */
	if( number_of_relocation_entries > 0 )
	{
/* TODO print relation table entries */
	}
	return( 1 );
}

/* Reads the extended header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_extended_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t extended_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	uint8_t extended_header_data[ 2 ];

	static char *function = "libexe_io_handle_read_extended_header";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading extended header at offset: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 extended_header_offset,
		 extended_header_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     (off64_t) extended_header_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek extended header offset: %" PRIu32 ".",
		 function,
		 extended_header_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              extended_header_data,
	              2,
	              error );

	if( read_count != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read first 2 bytes of extended header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extended header data:\n",
		 function );
		libcnotify_print_data(
		 extended_header_data,
		 2,
		 0 );
	}
#endif
/* TODO pass extended header, so it is read once */
	if( ( extended_header_data[ 0 ] == (uint8_t) 'L' )
	 && ( extended_header_data[ 1 ] == (uint8_t) 'E' ) )
	{
		if( libexe_io_handle_read_le_header(
		     io_handle,
		     file_io_handle,
		     extended_header_offset,
		     number_of_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read LE header.",
			 function );

			return( -1 );
		}
	}
	else if( ( extended_header_data[ 0 ] == (uint8_t) 'N' )
	      && ( extended_header_data[ 1 ] == (uint8_t) 'E' ) )
	{
		if( libexe_io_handle_read_ne_header(
		     io_handle,
		     file_io_handle,
		     extended_header_offset,
		     number_of_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read NE header.",
			 function );

			return( -1 );
		}
	}
	else if( ( extended_header_data[ 0 ] == (uint8_t) 'P' )
	      && ( extended_header_data[ 1 ] == (uint8_t) 'E' ) )
	{
		if( libexe_io_handle_read_pe_header(
		     io_handle,
		     file_io_handle,
		     extended_header_offset,
		     number_of_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read PE/COFF header.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extended header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the LE header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_le_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t le_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	exe_le_header_t le_header;

	static char *function = "libexe_io_handle_read_le_header";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( number_of_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading LE header at offset: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 le_header_offset,
		 le_header_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     (off64_t) le_header_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek LE header offset: %" PRIu32 ".",
		 function,
		 le_header_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &le_header,
	              sizeof( exe_le_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( exe_le_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read LE header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: LE header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &le_header,
		 sizeof( exe_le_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     le_header.signature,
	     exe_le_signature,
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 le_header.signature[ 0 ],
		 le_header.signature[ 1 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif

/* TODO */
	io_handle->executable_type = LIBEXE_EXECUTABLE_TYPE_LE;

	*number_of_sections = 0;

	return( 1 );
}

/* Reads the NE header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_ne_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t ne_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	exe_ne_header_t ne_header;

	static char *function = "libexe_io_handle_read_ne_header";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( number_of_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading NE header at offset: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 ne_header_offset,
		 ne_header_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     (off64_t) ne_header_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek NE header offset: %" PRIu32 ".",
		 function,
		 ne_header_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &ne_header,
	              sizeof( exe_ne_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( exe_ne_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read NE header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: NE header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &ne_header,
		 sizeof( exe_ne_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ne_header.signature,
	     exe_ne_signature,
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ne_header.signature[ 0 ],
		 ne_header.signature[ 1 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif

/* TODO */
	io_handle->executable_type = LIBEXE_EXECUTABLE_TYPE_NE;

	*number_of_sections = 0;

	return( 1 );
}

/* Reads the PE/COFF header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_pe_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t pe_header_offset,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	exe_pe_header_t pe_header;

	static char *function = "libexe_io_handle_read_pe_header";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading PE header at offset: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 pe_header_offset,
		 pe_header_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     (off64_t) pe_header_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek PE header offset: %" PRIu32 ".",
		 function,
		 pe_header_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &pe_header,
	              sizeof( exe_pe_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( exe_pe_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read PE header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: PE header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &pe_header,
		 sizeof( exe_pe_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     pe_header.signature,
	     exe_pe_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\\x%" PRIx8 "\\x%" PRIx8 "\n",
		 function,
		 pe_header.signature[ 0 ],
		 pe_header.signature[ 1 ],
		 pe_header.signature[ 2 ],
		 pe_header.signature[ 3 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libexe_io_handle_read_coff_header(
	     io_handle,
	     file_io_handle,
	     number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COFF header.",
		 function );

		return( -1 );
	}
	io_handle->executable_type = LIBEXE_EXECUTABLE_TYPE_PE_COFF;

	return( 1 );
}

/* Reads the COFF header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_coff_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_sections,
     libcerror_error_t **error )
{
	exe_coff_header_t coff_header;

	static char *function                 = "libexe_io_handle_read_coff_header";
	ssize_t read_count                    = 0;
	uint16_t optional_header_size         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t posix_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	uint32_t value_32bit                  = 0;
	uint16_t value_16bit                  = 0;
	int result                            = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( number_of_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sections.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &coff_header,
	              sizeof( exe_coff_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( exe_coff_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COFF header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: COFF header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &coff_header,
		 sizeof( exe_coff_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 coff_header.number_of_sections,
	 *number_of_sections );

	byte_stream_copy_to_uint32_little_endian(
	 coff_header.creation_time,
	 io_handle->creation_time );

	byte_stream_copy_to_uint16_little_endian(
	 coff_header.optional_header_size,
	 optional_header_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 coff_header.target_architecture_type,
		 value_16bit );
		libcnotify_printf(
		 "%s: target architecture type\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of sections\t\t\t: %" PRIu16 "\n",
		 function,
		 *number_of_sections );

		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_byte_stream(
		     posix_time,
		     coff_header.creation_time,
		     4,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy POSIX time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) posix_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: creation time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 posix_time_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 coff_header.symbol_table_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: symbol table offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 coff_header.number_of_symbols,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of symbols\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: optional header size\t\t\t: %" PRIu16 "\n",
		 function,
		 optional_header_size );

		byte_stream_copy_to_uint16_little_endian(
		 coff_header.characteristic_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: characteristic flags\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libexe_debug_print_file_characteristic_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libexe_io_handle_read_coff_optional_header(
	     io_handle,
	     file_io_handle,
	     optional_header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COFF optional header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the COFF optional header
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_coff_optional_header(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t optional_header_size,
     libcerror_error_t **error )
{
	libexe_data_directory_descriptor_t *data_directory_descriptor = NULL;
	uint8_t *coff_optional_header                                 = NULL;
	uint8_t *coff_optional_header_data                            = NULL;
	static char *function                                         = "libexe_io_handle_read_coff_optional_header";
	ssize_t read_count                                            = 0;
	uint32_t number_of_data_directories_entries                   = 0;
	uint16_t signature                                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                          = 0;
	uint32_t value_32bit                                          = 0;
	uint16_t value_16bit                                          = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	coff_optional_header = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * optional_header_size );

	if( coff_optional_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COFF optional header.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              coff_optional_header,
	              (size_t) optional_header_size,
	              error );

	if( read_count != (ssize_t) optional_header_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COFF optional header.",
		 function );

		goto on_error;
	}
	coff_optional_header_data = coff_optional_header;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: COFF optional header:\n",
		 function );
		libcnotify_print_data(
		 coff_optional_header_data,
		 (size_t) optional_header_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (exe_coff_optional_header_t *) coff_optional_header_data )->signature,
	 signature );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 signature );

		libcnotify_printf(
		 "%s: major linker version\t\t: %" PRIu8 "\n",
		 function,
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->major_linker_version );

		libcnotify_printf(
		 "%s: minor linker version\t\t: %" PRIu8 "\n",
		 function,
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->minor_linker_version );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->text_section_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: text section size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->initialized_data_section_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: initialized data section size\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->uninitialized_data_section_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: uninitialized data section size\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->entry_point_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: entry point offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_t *) coff_optional_header_data )->code_base_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: code base offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		if( signature == LIBEXE_COFF_OPTIONAL_HEADER_SIGNATURE_PE32 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_t *) coff_optional_header_data )->data_base_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: data base offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( ( signature != LIBEXE_COFF_OPTIONAL_HEADER_SIGNATURE_PE32 )
	 && ( signature != LIBEXE_COFF_OPTIONAL_HEADER_SIGNATURE_PE32_PLUS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		goto on_error;
	}
	coff_optional_header_data += sizeof( exe_coff_optional_header_t );

	if( signature == LIBEXE_COFF_OPTIONAL_HEADER_SIGNATURE_PE32 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->number_of_data_directories_entries,
		 number_of_data_directories_entries );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->image_base_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: image base offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->section_alignment_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: section alignment size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->file_alignment_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: file alignment size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->major_operating_system_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major operating system version\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->minor_operating_system_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor operating system version\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->major_image_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major image version\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->minor_image_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor image version\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->major_subsystem_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major subsystem version\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->minor_subsystem_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor subsystem version\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->image_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: image size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->headers_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: headers size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->subsystem,
			 value_16bit );
			libcnotify_printf(
			 "%s: subsystem\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->dll_characteristic_flags,
			 value_16bit );
			libcnotify_printf(
			 "%s: DLL characteristic flags\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
			libexe_debug_print_dll_characteristic_flags(
			 value_16bit );
			libcnotify_printf(
			 "\n" );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->stack_reservation_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: stack reservation size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->stack_commit_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: stack commit size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->heap_reservation_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: heap reservation size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->heap_commit_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: heap commit size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_t *) coff_optional_header_data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: number of data directory entries\t: %" PRIu32 "\n",
			 function,
			 number_of_data_directories_entries );
		}
#endif
		coff_optional_header_data += sizeof( exe_coff_optional_header_pe32_t );
	}
	else if( signature == LIBEXE_COFF_OPTIONAL_HEADER_SIGNATURE_PE32 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->number_of_data_directories_entries,
		 number_of_data_directories_entries );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->image_base_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: image base offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->section_alignment_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: section alignment size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->file_alignment_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: file alignment size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->major_operating_system_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major operating system version\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->minor_operating_system_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor operating system version\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->major_image_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major image version\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->minor_image_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor image version\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->major_subsystem_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: major subsystem version\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->minor_subsystem_version,
			 value_16bit );
			libcnotify_printf(
			 "%s: minor subsystem version\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->image_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: image size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->headers_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: headers size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->subsystem,
			 value_16bit );
			libcnotify_printf(
			 "%s: subsystem\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->dll_characteristic_flags,
			 value_16bit );
			libcnotify_printf(
			 "%s: DLL characteristic flags\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
			libexe_debug_print_dll_characteristic_flags(
			 value_16bit );
			libcnotify_printf(
			 "\n" );

			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->stack_reservation_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: stack reservation size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->stack_commit_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: stack commit size\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->heap_reservation_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: heap reservation size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->heap_commit_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: heap commit size\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_pe32_plus_t *) coff_optional_header_data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: number of data directory entries\t: %" PRIu32 "\n",
			 function,
			 number_of_data_directories_entries );
		}
#endif
		coff_optional_header_data += sizeof( exe_coff_optional_header_pe32_plus_t );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( number_of_data_directories_entries > 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of data directory entries: %" PRIu32 ".",
		 function,
		 number_of_data_directories_entries );

		goto on_error;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_EXPORT_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->export_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->export_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: export table RVA\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: export table size\t\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_IMPORT_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->import_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->import_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: import table RVA\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: import table size\t\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_RESOURCE_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->resource_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->resource_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: resource table RVA\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: resource table size\t\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_EXCEPTION_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->exception_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->exception_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: exception table RVA\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: exception table size\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_CERTIFICATE_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->certificate_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->certificate_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: certificate table RVA\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: certificate table size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_BASE_RELOCATION_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->base_relocation_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->base_relocation_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: base relocation table RVA\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: base relocation table size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_DEBUG_DATA ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->debug_data_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->debug_data_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: debug data RVA\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: debug data size\t\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->architecture_specific_data_rva,
			 value_32bit );
			libcnotify_printf(
			 "%s: architecture-specific data RVA\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->architecture_specific_data_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: architecture-specific data size\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->global_pointer_register,
			 value_32bit );
			libcnotify_printf(
			 "%s: global pointer register\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_THREAD_LOCAL_STORAGE_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->thread_local_storage_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->thread_local_storage_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: thread local storage table RVA\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: thread local storage table size\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_LOAD_CONFIGURATION_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->load_configuration_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->load_configuration_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: load configuration table RVA\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: load configuration table size\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_IMPORT_ADDRESS_TABLE ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->import_address_table_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->import_address_table_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: import address table RVA\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: import address table size\t\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_DELAY_IMPORT_DESCRIPTOR ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->delay_import_descriptor_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->delay_import_descriptor_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: delay import descriptor RVA\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: delay import descriptor size\t: %" PRIu32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
		data_directory_descriptor = &( io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_COM_PLUS_RUNTIME_HEADER ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->com_plus_runtime_header_rva,
		 data_directory_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->com_plus_runtime_header_size,
		 data_directory_descriptor->size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: COM+ runtime header RVA\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: COM+ runtime header size\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_directory_descriptor->size );
		}
#endif
		number_of_data_directories_entries--;
	}
	if( number_of_data_directories_entries > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (exe_coff_optional_header_data_directories_t *) coff_optional_header_data )->unknown4,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		number_of_data_directories_entries--;
	}
	memory_free(
	 coff_optional_header );

	return( 1 );

on_error:
	if( coff_optional_header != NULL )
	{
		memory_free(
		 coff_optional_header );
	}
	return( -1 );
}

/* Reads the section table
 * Returns 1 if successful or -1 on error
 */
int libexe_io_handle_read_section_table(
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t number_of_sections,
     libcdata_array_t *sections_array,
     libcerror_error_t **error )
{
	libexe_section_descriptor_t *section_descriptor = NULL;
	uint8_t *section_table                          = NULL;
	uint8_t *section_table_data                     = NULL;
	static char *function                           = "libexe_io_handle_read_section_table";
	size_t section_table_size                       = 0;
	ssize_t read_count                              = 0;
	uint32_t section_data_offset                    = 0;
	uint32_t section_data_size                      = 0;
	uint16_t section_index                          = 0;
	int entry_index                                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                            = 0;
	uint16_t value_16bit                            = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	section_table_size = sizeof( exe_section_table_entry_t )
	                   * number_of_sections;

	if( section_table_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section table size value out of bounds.",
		 function );

		return( -1 );
	}
	section_table = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * section_table_size );

	if( section_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section table.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              section_table,
	              section_table_size,
	              error );

	if( read_count != (ssize_t) section_table_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section table.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: section table data:\n",
		 function );
		libcnotify_print_data(
		 section_table,
		 section_table_size,
		 0 );
	}
#endif
	section_table_data = section_table;

	while( section_table_size >= sizeof( exe_section_table_entry_t ) )
	{
		if( libexe_section_descriptor_initialize(
		     &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create section descriptor.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     section_descriptor->name,
		     ( (exe_section_table_entry_t *) section_table_data )->name,
		     8 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
		section_descriptor->name[ 8 ] = 0;

		section_descriptor->name_size = libcstring_narrow_string_length(
		                                 section_descriptor->name );

		if( section_descriptor->name_size > 0 )
		{
			section_descriptor->name_size += 1;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_section_table_entry_t *) section_table_data )->virtual_address,
		 section_descriptor->virtual_address );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_section_table_entry_t *) section_table_data )->data_size,
		 section_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (exe_section_table_entry_t *) section_table_data )->data_offset,
		 section_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " name\t\t\t\t: %s\n",
			 function,
			 section_index,
			 section_descriptor->name );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->virtual_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " virtual size\t\t\t: %" PRIu32 "\n",
			 function,
			 section_index,
			 value_32bit );

			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " virtual address\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_index,
			 section_descriptor->virtual_address );

			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " data size\t\t\t: %" PRIu32 "\n",
			 function,
			 section_index,
			 section_data_size );

			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " data offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_index,
			 section_data_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->relocations_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " relocations offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->line_numbers_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " line numbers offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_index,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->relocations_offset,
			 value_16bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " number of relocations\t\t: %" PRIu16 "\n",
			 function,
			 section_index,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->line_numbers_offset,
			 value_16bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " number of line numbers\t\t: %" PRIu16 "\n",
			 function,
			 section_index,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (exe_section_table_entry_t *) section_table_data )->section_characteristic_flags,
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02" PRIu16 " section characteristic flags\t: 0x%08" PRIx32 "\n",
			 function,
			 section_index,
			 value_32bit );
			libexe_debug_print_section_characteristic_flags(
			 value_32bit );
			libcnotify_printf(
			 "\n" );
		}
#endif
		section_table_data += sizeof( exe_section_table_entry_t );
		section_table_size -= sizeof( exe_section_table_entry_t );

		if( libexe_section_descriptor_set_data_range(
		     section_descriptor,
		     (off64_t) section_data_offset,
		     (size64_t) section_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data range in section descriptor.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     sections_array,
		     &entry_index,
		     (intptr_t *) section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append section descriptor to sections array.",
			 function );

			goto on_error;
		}
		section_descriptor = NULL;

		section_index++;
	}
	memory_free(
	 section_table );

	return( 1 );

on_error:
	if( section_descriptor != NULL )
	{
		libexe_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	if( section_table != NULL )
	{
		memory_free(
		 section_table );
	}
	return( -1 );
}

/* Reads the segment data into the buffer
 * Callback function for the section stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libexe_io_handle_read_segment_data(
         intptr_t *data_handle LIBEXE_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBEXE_ATTRIBUTE_UNUSED,
         int segment_file_index LIBEXE_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBEXE_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBEXE_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "libexe_io_handle_read_segment_data";
	ssize_t read_count    = 0;

	LIBEXE_UNREFERENCED_PARAMETER( data_handle )
	LIBEXE_UNREFERENCED_PARAMETER( segment_index )
	LIBEXE_UNREFERENCED_PARAMETER( segment_file_index )
	LIBEXE_UNREFERENCED_PARAMETER( segment_flags )
	LIBEXE_UNREFERENCED_PARAMETER( read_flags )

	read_count = libbfio_handle_read_buffer(
		      file_io_handle,
		      segment_data,
		      segment_data_size,
		      error );

	if( read_count != (ssize_t) segment_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain segment offset
 * Callback function for the section stream
 * Returns the offset or -1 on error
 */
off64_t libexe_io_handle_seek_segment_offset(
         intptr_t *data_handle LIBEXE_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBEXE_ATTRIBUTE_UNUSED,
         int segment_file_index LIBEXE_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libexe_io_handle_seek_segment_offset";

	LIBEXE_UNREFERENCED_PARAMETER( data_handle )
	LIBEXE_UNREFERENCED_PARAMETER( segment_index )
	LIBEXE_UNREFERENCED_PARAMETER( segment_file_index )

	segment_offset = libbfio_handle_seek_offset(
	                  file_io_handle,
	                  segment_offset,
	                  SEEK_SET,
	                  error );

	if( segment_offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek segment offset.",
		 function );

		return( -1 );
	}
	return( segment_offset );
}

