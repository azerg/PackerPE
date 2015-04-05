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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libexe_definitions.h"
#include "libexe_io_handle.h"
#include "libexe_libbfio.h"
#include "libexe_libcerror.h"
#include "libexe_libfcache.h"
#include "libexe_libfdata.h"
#include "libexe_libuna.h"
#include "libexe_section.h"
#include "libexe_section_io_handle.h"

/* Creates a section
 * Make sure the value section is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libexe_section_initialize(
     libexe_section_t **section,
     libexe_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libexe_section_descriptor_t *section_descriptor,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_initialize";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( *section != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section value already set.",
		 function );

		return( -1 );
	}
	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	internal_section = memory_allocate_structure(
	                    libexe_internal_section_t );

	if( internal_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal section.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_section,
	     0,
	     sizeof( libexe_internal_section_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal section.",
		 function );

		memory_free(
		 internal_section );

		return( -1 );
	}
	internal_section->file_io_handle     = file_io_handle;
	internal_section->io_handle          = io_handle;
	internal_section->section_descriptor = section_descriptor;

	*section = (libexe_section_t *) internal_section;

	return( 1 );

on_error:
	if( internal_section != NULL )
	{
		memory_free(
		 internal_section );
	}
	return( -1 );
}

/* Frees a section
 * Returns 1 if successful or -1 on error
 */
int libexe_section_free(
     libexe_section_t **section,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_free";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( *section != NULL )
	{
		internal_section = (libexe_internal_section_t *) *section;
		*section         = NULL;

		/* The file_io_handle, io_handle and section_descriptor references are freed elsewhere
		 */
		memory_free(
		 internal_section );
	}
	return( 1 );
}

/* Retrieves the size of the ASCII formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_name_size(
     libexe_section_t *section,
     size_t *name_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_name_size";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( name_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name size.",
		 function );

		return( -1 );
	}
	*name_size = internal_section->section_descriptor->name_size;

	return( 1 );
}

/* Retrieves the ASCII formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_name(
     libexe_section_t *section,
     char *name,
     size_t name_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_name";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( name_size < 1 )
	 || ( name_size < internal_section->section_descriptor->name_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid name value too small.",
		 function );

		return( -1 );
	}
	if( internal_section->section_descriptor->name_size == 0 )
	{
		name[ 0 ] = 0;
	}
	else
	{
		if( memory_copy(
		     name,
		     internal_section->section_descriptor->name,
		     internal_section->section_descriptor->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_utf8_name_size(
     libexe_section_t *section,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_utf8_name_size";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_byte_stream(
	     (uint8_t *) internal_section->section_descriptor->name,
	     internal_section->section_descriptor->name_size,
	     internal_section->io_handle->ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_utf8_name(
     libexe_section_t *section,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_utf8_name";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_byte_stream(
	     utf8_string,
	     utf8_string_size,
	     (uint8_t *) internal_section->section_descriptor->name,
	     internal_section->section_descriptor->name_size,
	     internal_section->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_utf16_name_size(
     libexe_section_t *section,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_utf16_name_size";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_byte_stream(
	     (uint8_t *) internal_section->section_descriptor->name,
	     internal_section->section_descriptor->name_size,
	     internal_section->io_handle->ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( -1 );
}

/* Retrieves the UTF-16 formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_utf16_name(
     libexe_section_t *section,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_utf16_name";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_byte_stream(
	     utf16_string,
	     utf16_string_size,
	     (uint8_t *) internal_section->section_descriptor->name,
	     internal_section->section_descriptor->name_size,
	     internal_section->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libexe_section_read_buffer(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_read_buffer";
	ssize_t read_count                          = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	read_count = libfdata_stream_read_buffer(
	              internal_section->section_descriptor->data_stream,
	              (intptr_t *) internal_section->file_io_handle,
	              buffer,
	              buffer_size,
	              0,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from section data stream.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Reads data at a specific offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libexe_section_read_buffer_at_offset(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libexe_section_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libexe_section_seek_offset(
	     section,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libexe_section_read_buffer(
	              section,
	              buffer,
	              buffer_size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libexe_section_seek_offset(
         libexe_section_t *section,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_seek_offset";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	offset = libfdata_stream_seek_offset(
	          internal_section->section_descriptor->data_stream,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek in section data stream.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the (current) offset
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_offset(
     libexe_section_t *section,
     off64_t *offset,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_offset";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_offset(
	     internal_section->section_descriptor->data_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section data stream offset.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_size(
     libexe_section_t *section,
     size64_t *size,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_size";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( libexe_section_descriptor_get_data_size(
	     internal_section->section_descriptor,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section descriptor data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the start offset
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_start_offset(
     libexe_section_t *section,
     off64_t *start_offset,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_start_offset";
	size64_t segment_size                       = 0;
	uint32_t segment_flags                      = 0;
	int segment_file_index                      = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_segment_by_index(
	     internal_section->section_descriptor->data_stream,
	     0,
	     &segment_file_index,
	     start_offset,
	     &segment_size,
	     &segment_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section data stream segment: 0.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the virtual address
 * Returns 1 if successful or -1 on error
 */
int libexe_section_get_virtual_address(
     libexe_section_t *section,
     uint32_t *virtual_address,
     libcerror_error_t **error )
{
	libexe_internal_section_t *internal_section = NULL;
	static char *function                       = "libexe_section_get_virtual_address";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	internal_section = (libexe_internal_section_t *) section;

	if( internal_section->section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid section - missing section descriptor.",
		 function );

		return( -1 );
	}
	if( virtual_address == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid virtual address.",
		 function );

		return( -1 );
	}
	*virtual_address = internal_section->section_descriptor->virtual_address;

	return( 1 );
}

/* Retrieves the section data file IO handle
 * Returns 1 if successful -1 on error
 */
int libexe_section_get_data_file_io_handle(
     libexe_section_t *section,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libexe_section_io_handle_t *io_handle = NULL;
	static char *function                 = "libexe_section_get_data_file_io_handle";

	if( libexe_section_io_handle_initialize(
	     &io_handle,
	     section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_initialize(
	     file_io_handle,
	     (intptr_t *) io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) libexe_section_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) libexe_section_io_handle_clone,
	     (int (*)(intptr_t *, int flags, libcerror_error_t **)) libexe_section_io_handle_open,
	     (int (*)(intptr_t *, libcerror_error_t **)) libexe_section_io_handle_close,
	     (ssize_t (*)(intptr_t *, uint8_t *, size_t, libcerror_error_t **)) libexe_section_io_handle_read,
	     (ssize_t (*)(intptr_t *, const uint8_t *, size_t, libcerror_error_t **)) libexe_section_io_handle_write,
	     (off64_t (*)(intptr_t *, off64_t, int, libcerror_error_t **)) libexe_section_io_handle_seek_offset,
	     (int (*)(intptr_t *, libcerror_error_t **)) libexe_section_io_handle_exists,
	     (int (*)(intptr_t *, libcerror_error_t **)) libexe_section_io_handle_is_open,
	     (int (*)(intptr_t *, size64_t *, libcerror_error_t **)) libexe_section_io_handle_get_size,
	     LIBBFIO_FLAG_IO_HANDLE_MANAGED | LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_FUNCTION,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( io_handle != NULL )
	{
		libexe_section_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( -1 );
}

