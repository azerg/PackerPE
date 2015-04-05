/*
 * Section decriptor functions
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

#include "libexe_io_handle.h"
#include "libexe_libcerror.h"
#include "libexe_libfcache.h"
#include "libexe_libfdata.h"
#include "libexe_section_descriptor.h"

/* Creates a section descriptor
 * Make sure the value section_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libexe_section_descriptor_initialize(
     libexe_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_descriptor_initialize";

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
	if( *section_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section descriptor value already set.",
		 function );

		return( -1 );
	}
	*section_descriptor = memory_allocate_structure(
	                       libexe_section_descriptor_t );

	if( *section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *section_descriptor,
	     0,
	     sizeof( libexe_section_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section descriptor.",
		 function );

		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;

		return( -1 );
	}
	if( libfdata_stream_initialize(
	     &( ( *section_descriptor )->data_stream ),
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libexe_io_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libexe_io_handle_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_resize(
	     ( *section_descriptor )->data_stream,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize data stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *section_descriptor != NULL )
	{
		if( ( *section_descriptor )->data_stream != NULL )
		{
			libfdata_stream_free(
			 &( ( *section_descriptor )->data_stream ),
			 NULL );
		}
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a section descriptor
 * Returns 1 if successful or -1 on error
 */
int libexe_section_descriptor_free(
     libexe_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_descriptor_free";
	int result            = 1;

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
	if( *section_descriptor != NULL )
	{
		if( libfdata_stream_free(
		     &( ( *section_descriptor )->data_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data stream.",
			 function );

			result = -1;
		}
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( result );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libexe_section_descriptor_get_data_size(
     libexe_section_descriptor_t *section_descriptor,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_descriptor_get_data_size";

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
	if( libfdata_stream_get_size(
	     section_descriptor->data_stream,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data stream size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data range
 * Returns 1 if successful or -1 on error
 */
int libexe_section_descriptor_get_data_range(
     libexe_section_descriptor_t *section_descriptor,
     off64_t *data_offset,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_descriptor_get_data_range";
	uint32_t segment_flags = 0;
	int segment_file_index = 0;

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
	if( libfdata_stream_get_segment_by_index(
	     section_descriptor->data_stream,
	     0,
	     &segment_file_index,
	     data_offset,
	     data_size,
	     &segment_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment from data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the data range
 * Returns 1 if successful or -1 on error
 */
int libexe_section_descriptor_set_data_range(
     libexe_section_descriptor_t *section_descriptor,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libexe_section_descriptor_set_data_range";

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
	if( libfdata_stream_set_segment_by_index(
	     section_descriptor->data_stream,
	     0,
	     0,
	     data_offset,
	     data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment in data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

