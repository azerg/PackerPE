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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libexe_data_directory_descriptor.h"
#include "libexe_codepage.h"
#include "libexe_debug.h"
#include "libexe_debug_data.h"
#include "libexe_definitions.h"
#include "libexe_export_table.h"
#include "libexe_import_table.h"
#include "libexe_io_handle.h"
#include "libexe_file.h"
#include "libexe_libbfio.h"
#include "libexe_libcdata.h"
#include "libexe_libcerror.h"
#include "libexe_libcnotify.h"
#include "libexe_libcstring.h"
#include "libexe_section.h"
#include "libexe_section_descriptor.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libexe_file_initialize(
     libexe_file_t **file,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libexe_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libexe_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 internal_file );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_file->sections_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sections array.",
		 function );

		goto on_error;
	}
	if( libexe_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	*file = (libexe_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		if( internal_file->sections_array != NULL )
		{
			libcdata_array_free(
			 &( internal_file->sections_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libexe_file_free(
     libexe_file_t **file,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_free";
	int result                            = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libexe_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libexe_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

		if( libcdata_array_free(
		     &( internal_file->sections_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libexe_section_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sections array.",
			 function );

			result = -1;
		}
		if( libexe_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals the file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libexe_file_signal_abort(
     libexe_file_t *file,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libexe_file_open(
     libexe_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_open";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEXE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     libcstring_narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libexe_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libexe_file_open_wide(
     libexe_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_open_wide";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEXE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     libcstring_wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libexe_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libexe_file_open_file_io_handle(
     libexe_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_open_file_io_handle";
	int bfio_access_flags                 = 0;
	int file_io_handle_is_open            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEXE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEXE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEXE_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		internal_file->file_io_handle_opened_in_library = 1;
	}
	if( libexe_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
	internal_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( ( file_io_handle_is_open == 0 )
	 && ( internal_file->file_io_handle_opened_in_library != 0 ) )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );

		internal_file->file_io_handle_opened_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libexe_file_close(
     libexe_file_t *file,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_close";
	int result                            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libexe_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libexe_io_handle_clear(
	     internal_file->io_handle,
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
	if( libcdata_array_resize(
	     internal_file->sections_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libexe_section_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize sections array.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libexe_file_open_read(
     libexe_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libexe_data_directory_descriptor_t *data_directory_descriptor = NULL;
	libexe_debug_data_t *debug_data                               = NULL;
	libexe_export_table_t *export_table                           = NULL;
	libexe_import_table_t *import_table                           = NULL;
	static char *function                                         = "libexe_file_open_read";
	off64_t file_offset                                           = 0;
	uint16_t number_of_sections                                   = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle->abort != 0 )
	{
		internal_file->io_handle->abort = 0;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libexe_io_handle_read_file_header(
	     internal_file->io_handle,
	     file_io_handle,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
	if( number_of_sections > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading section table:\n" );
		}
#endif
		if( libexe_io_handle_read_section_table(
		     internal_file->io_handle,
		     file_io_handle,
		     number_of_sections,
		     internal_file->sections_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section table.",
			 function );

			goto on_error;
		}
	}
	data_directory_descriptor = &( internal_file->io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_EXPORT_TABLE ] );

	if( data_directory_descriptor->size > 0 )
	{
		if( libexe_file_get_offset_by_relative_virtual_address(
		     internal_file,
		     data_directory_descriptor->virtual_address,
		     &file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset for relative virtual address: 0x%08" PRIx32 ".",
			 function,
			 data_directory_descriptor->virtual_address );

			goto on_error;
		}
		if( libexe_export_table_initialize(
		     &export_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create export table.",
			 function );

			goto on_error;
		}
		if( libexe_export_table_read(
		     export_table,
		     file_io_handle,
		     file_offset,
		     data_directory_descriptor->size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read export table.",
			 function );

			goto on_error;
		}
		if( libexe_export_table_free(
		     &export_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free export table.",
			 function );

			goto on_error;
		}
	}
	data_directory_descriptor = &( internal_file->io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_IMPORT_TABLE ] );

	if( data_directory_descriptor->size > 0 )
	{
		if( libexe_file_get_offset_by_relative_virtual_address(
		     internal_file,
		     data_directory_descriptor->virtual_address,
		     &file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset for relative virtual address: 0x%08" PRIx32 ".",
			 function,
			 data_directory_descriptor->virtual_address );

			goto on_error;
		}
		if( libexe_import_table_initialize(
		     &import_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create import table.",
			 function );

			goto on_error;
		}
		if( libexe_import_table_read(
		     import_table,
		     file_io_handle,
		     file_offset,
		     data_directory_descriptor->size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read import table.",
			 function );

			goto on_error;
		}
		if( libexe_import_table_free(
		     &import_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free import table.",
			 function );

			goto on_error;
		}
	}
	data_directory_descriptor = &( internal_file->io_handle->data_directories[ LIBEXE_DATA_DIRECTORY_DEBUG_DATA ] );

	if( data_directory_descriptor->size > 0 )
	{
		if( libexe_file_get_offset_by_relative_virtual_address(
		     internal_file,
		     data_directory_descriptor->virtual_address,
		     &file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset for relative virtual address: 0x%08" PRIx32 ".",
			 function,
			 data_directory_descriptor->virtual_address );

			goto on_error;
		}
		if( libexe_debug_data_initialize(
		     &debug_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create debug data.",
			 function );

			goto on_error;
		}
		if( libexe_debug_data_read(
		     debug_data,
		     file_io_handle,
		     file_offset,
		     data_directory_descriptor->size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read debug data.",
			 function );

			goto on_error;
		}
		if( libexe_debug_data_free(
		     &debug_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free debug data.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( debug_data != NULL )
	{
		libexe_debug_data_free(
		 &debug_data,
		 NULL );
	}
	if( import_table != NULL )
	{
		libexe_import_table_free(
		 &import_table,
		 NULL );
	}
	if( export_table != NULL )
	{
		libexe_export_table_free(
		 &export_table,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libexe_file_get_ascii_codepage(
     libexe_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_get_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ascii_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	*ascii_codepage = internal_file->io_handle->ascii_codepage;

	return( 1 );
}

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libexe_file_set_ascii_codepage(
     libexe_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_set_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( ascii_codepage != LIBEXE_CODEPAGE_ASCII )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_874 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_932 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_936 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_949 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_950 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1250 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1251 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1252 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1253 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1254 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1255 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1256 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1257 )
	 && ( ascii_codepage != LIBEXE_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Retrieves offset of a relative virtual address
 * Returns 1 if successful, 0 if no such section or -1 on error
 */
int libexe_file_get_offset_by_relative_virtual_address(
     libexe_internal_file_t *internal_file,
     uint32_t virtual_address,
     off64_t *offset,
     libcerror_error_t **error )
{
	libexe_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libexe_file_get_offset_by_relative_virtual_address";
	size64_t section_size                           = 0;
	int number_of_sections                          = 0;
	int section_index                               = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file->sections_array,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sections.",
		 function );

		return( -1 );
	}
	for( section_index = 0;
	     section_index < number_of_sections;
	     section_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file->sections_array,
		     section_index,
		     (intptr_t **) &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section descriptor: %d.",
			 function,
			 section_index );

			return( -1 );
		}
		if( libexe_section_descriptor_get_data_range(
		     section_descriptor,
		     offset,
		     &section_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section descriptor: %d data range.",
			 function,
			 section_index );

			return( -1 );
		}
		if( ( virtual_address >= section_descriptor->virtual_address )
		 && ( ( virtual_address - section_descriptor->virtual_address ) < section_size ) )
		{
			*offset += virtual_address - section_descriptor->virtual_address;

			return( 1 );
		}
	}
	*offset = 0;

	return( 0 );
}

/* Retrieves the number of sections
 * Returns 1 if successful or -1 on error
 */
int libexe_file_get_number_of_sections(
     libexe_file_t *file,
     int *number_of_sections,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file = NULL;
	static char *function                 = "libexe_file_get_number_of_sections";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( libcdata_array_get_number_of_entries(
	     internal_file->sections_array,
	     number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sections.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific section
 * Returns 1 if successful or -1 on error
 */
int libexe_file_get_section(
     libexe_file_t *file,
     int section_index,
     libexe_section_t **section,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file           = NULL;
	libexe_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libexe_file_get_section";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

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
	if( libcdata_array_get_entry_by_index(
	     internal_file->sections_array,
	     section_index,
	     (intptr_t **) &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section descriptor: %d.",
		 function,
		 section_index );

		return( -1 );
	}
	if( libexe_section_initialize(
	     section,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific section by an ASCII formatted name
 * Returns 1 if successful, 0 if no such section or -1 on error
 */
int libexe_file_get_section_by_name(
     libexe_file_t *file,
     const char *name,
     size_t name_length,
     libexe_section_t **section,
     libcerror_error_t **error )
{
	libexe_internal_file_t *internal_file           = NULL;
	libexe_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libexe_file_get_section_by_name";
	int number_of_sections                          = 0;
	int section_index                               = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libexe_internal_file_t *) file;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
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
	if( libcdata_array_get_number_of_entries(
	     internal_file->sections_array,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sections.",
		 function );

		return( -1 );
	}
	for( section_index = 0;
	     section_index < number_of_sections;
	     section_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file->sections_array,
		     section_index,
		     (intptr_t **) &section_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section descriptor: %d.",
			 function,
			 section_index );

			return( -1 );
		}
		if( section_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section descriptor: %d.",
			 function,
			 section_index );

			return( -1 );
		}
		if( ( name_length + 1 ) == section_descriptor->name_size )
		{
			if( libcstring_narrow_string_compare(
			     section_descriptor->name,
			     name,
			     name_length ) == 0 )
			{
				if( libexe_section_initialize(
				     section,
				     internal_file->io_handle,
				     internal_file->file_io_handle,
				     section_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create section.",
					 function );

					return( -1 );
				}
				return( 1 );
			}
		}
	}
	return( 0 );
}

