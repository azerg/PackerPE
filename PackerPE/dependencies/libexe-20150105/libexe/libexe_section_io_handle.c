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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libexe_definitions.h"
#include "libexe_libbfio.h"
#include "libexe_libcerror.h"
#include "libexe_section.h"
#include "libexe_section_io_handle.h"
#include "libexe_types.h"

/* Creates a section IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libexe_section_io_handle_initialize(
     libexe_section_io_handle_t **io_handle,
     libexe_section_t *section,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_initialize";

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
	*io_handle = memory_allocate_structure(
	              libexe_section_io_handle_t );

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
	     sizeof( libexe_section_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->section = section;

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

/* Frees a section IO handle
 * Returns 1 if succesful or -1 on error
 */
int libexe_section_io_handle_free(
     libexe_section_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_free";

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
		/* The section reference is freed elsewhere
		 */
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

/* Clones (duplicates) the IO handle and its attributes
 * Returns 1 if succesful or -1 on error
 */
int libexe_section_io_handle_clone(
     libexe_section_io_handle_t **destination_io_handle,
     libexe_section_io_handle_t *source_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_clone";

	if( destination_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination IO handle already set.",
		 function );

		return( -1 );
	}
	if( source_io_handle == NULL )
	{
		*destination_io_handle = NULL;

		return( 1 );
	}
	if( libexe_section_io_handle_initialize(
	     destination_io_handle,
	     source_io_handle->section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		return( -1 );
	}
	( *destination_io_handle )->access_flags = source_io_handle->access_flags;

	return( 1 );
}

/* Opens the IO handle
 * Returns 1 if successful or -1 on error
 */
int libexe_section_io_handle_open(
     libexe_section_io_handle_t *io_handle,
     int flags,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_open";

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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: IO handle already open.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	/* Currently only support for reading data
	 */
	if( ( flags & ~( LIBBFIO_ACCESS_FLAG_READ ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	io_handle->access_flags = flags;
	io_handle->is_open      = 1;

	return( 1 );
}

/* Closes the IO handle
 * Returns 0 if successful or -1 on error
 */
int libexe_section_io_handle_close(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_close";

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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - not open.",
		 function );

		return( -1 );
	}
	io_handle->is_open = 0;

	return( 0 );
}

/* Reads a buffer from the IO handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libexe_section_io_handle_read(
         libexe_section_io_handle_t *io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_read";
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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - not open.",
		 function );

		return( -1 );
	}
	if( ( io_handle->access_flags & LIBBFIO_ACCESS_FLAG_READ ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - no read access.",
		 function );

		return( -1 );
	}
	read_count = libexe_section_read_buffer(
	              io_handle->section,
	              buffer,
	              size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from section.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a buffer to the IO handle
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libexe_section_io_handle_write(
         libexe_section_io_handle_t *io_handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_write";

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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - not open.",
		 function );

		return( -1 );
	}
	if( ( io_handle->access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - no write access.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Seeks a certain offset within the IO handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libexe_section_io_handle_seek_offset(
         libexe_section_io_handle_t *io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_seek_offset";

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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - not open.",
		 function );

		return( -1 );
	}
	offset = libexe_section_seek_offset(
	          io_handle->section,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in section.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Function to determine if the attached file exists
 * Returns 1 if the attached file exists, 0 if not or -1 on error
 */
int libexe_section_io_handle_exists(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_exists";

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
	if( io_handle->section == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Check if the attached file is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int libexe_section_io_handle_is_open(
     libexe_section_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_is_open";

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
	if( io_handle->section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing section.",
		 function );

		return( -1 );
	}
	if( io_handle->is_open == 0 )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves the attached file size
 * Returns 1 if successful or -1 on error
 */
int libexe_section_io_handle_get_size(
     libexe_section_io_handle_t *io_handle,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_io_handle_get_size";

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
	if( libexe_section_get_size(
	     io_handle->section,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

