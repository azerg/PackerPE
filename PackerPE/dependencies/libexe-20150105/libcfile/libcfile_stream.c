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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_SYS_STAT_H ) || defined( WINAPI )
#include <sys/stat.h>
#endif

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_GLIB_H )
#include <glib.h>
#include <glib/gstdio.h>
#endif

#include <stdio.h>

#include "libcfile_definitions.h"
#include "libcfile_libcerror.h"
#include "libcfile_libclocale.h"
#include "libcfile_libcstring.h"
#include "libcfile_stream.h"
#include "libcfile_types.h"

/* Creates a file stream
 * Make sure the value stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_initialize(
     libcfile_stream_t **stream,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_initialize";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( *stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid stream value already set.",
		 function );

		return( -1 );
	}
	internal_stream = memory_allocate_structure(
	                   libcfile_internal_stream_t );

	if( internal_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stream.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_stream,
	     0,
	     sizeof( libcfile_internal_stream_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stream.",
		 function );

		goto on_error;
	}
	*stream = (libcfile_stream_t *) internal_stream;

	return( 1 );

on_error:
	if( internal_stream != NULL )
	{
		memory_free(
		 internal_stream );
	}
	return( -1 );
}

/* Frees a file stream
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_free(
     libcfile_stream_t **stream,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_free";
	int result                                  = 1;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( *stream != NULL )
	{
		internal_stream = (libcfile_internal_stream_t *) *stream;

		if( internal_stream->stream != NULL )
		{
			if( libcfile_stream_close(
			     *stream,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close stream.",
				 function );

				result = -1;
			}
		}
		*stream = NULL;

		memory_free(
		 internal_stream );
	}
	return( result );
}

#if defined( HAVE_FOPEN ) || defined( WINAPI )

/* Opens a file stream
 * This function uses the POSIX fopen function or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_open(
     libcfile_stream_t *stream,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	const char *stream_io_mode                  = NULL;
	static char *function                       = "libcfile_stream_open";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( ( ( access_flags & LIBCFILE_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBCFILE_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		if( ( access_flags & LIBCFILE_ACCESS_FLAG_TRUNCATE ) != 0 )
		{
			stream_io_mode = "wb+";
		}
		else
		{
			stream_io_mode = "ab+";
		}
	}
	else if( ( access_flags & LIBCFILE_ACCESS_FLAG_READ ) != 0 )
	{
		stream_io_mode = "rb";
	}
	else if( ( access_flags & LIBCFILE_ACCESS_FLAG_WRITE ) != 0 )
	{
		if( ( access_flags & LIBCFILE_ACCESS_FLAG_TRUNCATE ) != 0 )
		{
			stream_io_mode = "wb";
		}
		else
		{
			stream_io_mode = "ab";
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags: 0x%02x.",
		 function,
		 access_flags );

		return( -1 );
	}
#if defined( HAVE_GLIB_H )
	internal_stream->stream = g_fopen(
	                           filename,
	                           stream_io_mode );
#else
	internal_stream->stream = fopen(
	                           filename,
	                           stream_io_mode );
#endif
	if( internal_stream->stream == NULL )
	{
		switch( errno )
		{
			case EACCES:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 filename );

				break;

			case ENOENT:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 filename );

				break;

			default:
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 errno,
				 "%s: unable to open file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 filename );

				break;
		}
		return( -1 );
	}
	return( 1 );
}

#else
#error Missing file stream open function
#endif

/* TODO */
/* wide character FILE stream open
 */
#if defined( WINAPI )
#define file_stream_open_wide( filename, mode ) \
	_wfopen( filename, mode )
#endif

#if defined( HAVE_FCLOSE ) || defined( WINAPI )

/* Closes the file stream
 * This function uses the POSIX fclose function or equivalent
 * Returns 0 if successful or -1 on error
 */
int libcfile_stream_close(
     libcfile_stream_t *stream,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_close";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream != NULL )
	{
		if( fclose(
		     internal_stream->stream ) != 0 )
		{
			libcerror_system_set_error(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 errno,
			 "%s: unable to close stream.",
			 function );

			return( -1 );
		}
		internal_stream->stream = NULL;
	}
	return( 0 );
}

#else
#error Missing file stream close function
#endif

#if defined( HAVE_FREAD ) || defined( WINAPI )

/* Reads a buffer from the file stream
 * This function uses the POSIX fread function or equivalent
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libcfile_stream_read_buffer(
         libcfile_stream_t *stream,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_read_buffer";
	size_t read_count                           = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
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
	read_count = fread(
	              (void *) buffer,
	              1,
	              size,
	              internal_stream->stream );

	if( ( read_count == 0 )
	 || ( read_count != size ) )
	{
		if( feof(
		     internal_stream->stream ) == 0 )
		{
			libcerror_system_set_error(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 errno,
			 "%s: unable to read from stream.",
			 function );

			clearerr(
			 internal_stream->stream );

			return( -1 );
		}
		clearerr(
		 internal_stream->stream );
	}
	return( (ssize_t) read_count );
}

#else
#error Missing file read function
#endif

#if defined( HAVE_FWRITE ) || defined( WINAPI )

/* Writes a buffer to the file stream
 * This function uses the POSIX fwrite function or equivalent
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libcfile_stream_write_buffer(
         libcfile_stream_t *stream,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_write_buffer";
	ssize_t write_count                         = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
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
	write_count = fwrite(
	               (void *) buffer,
	               1,
	               size,
	               internal_stream->stream );

	if( write_count == 0 )
	{
		if( feof(
		     internal_stream->stream ) == 0 )
		{
			libcerror_system_set_error(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 errno,
			 "%s: unable to write to stream.",
			 function );

			clearerr(
			 internal_stream->stream );

			return( -1 );
		}
		clearerr(
		 internal_stream->stream );
	}
	return( write_count );
}

#else
#error Missing file stream write function
#endif

#if ( defined( HAVE_FSEEKO ) && defined( HAVE_FTELLO ) ) || ( defined( HAVE_FSEEKO64 ) && defined( HAVE_FTELLO64 ) ) || defined( WINAPI )

/* Seeks a certain offset within the file stream
 * This function uses the POSIX fseeko and ftello functions or equivalent
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libcfile_stream_seek_offset(
         libcfile_stream_t *stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_seek_offset";
	int result                                  = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
		 function );

		return( -1 );
	}
#if defined( WINAPI ) && ( LONG_MAX < INT64_MAX )
	if( offset > (off64_t) LONG_MAX )
#else
	if( offset > (off64_t) INT64_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	result = fseek(
	          internal_stream->stream,
	          (long) offset,
	          whence );
#elif defined( HAVE_FSEEKO64 ) && !defined( HAVE_FSEEKO )
	result = fseeko64(
	          internal_stream->stream,
	          offset,
	          whence );
#else
	result = fseeko(
	          internal_stream->stream,
	          (off_t) offset,
	          whence );
#endif
	if( result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 errno,
		 "%s: unable to seek offset in stream.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	offset = (off64_t) ftell(
	                    internal_stream->stream );
#elif defined( HAVE_FTELLO64 ) && !defined( HAVE_FTELLO )
	offset = ftello64(
	          internal_stream->stream );
#else
	offset = (off64_t) ftello(
	                    internal_stream->stream );
#endif
	if( offset < 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 errno,
		 "%s: unable to get offset from stream.",
		 function );

		return( -1 );
	}
	return( offset );
}

#else
#error Missing file fstream seeko and ftello function
#endif

#if defined( HAVE_FTELLO ) || defined( HAVE_FTELLO64 ) || defined( WINAPI )

/* Retrieves the current offset in the file stream
 * This function uses the POSIX ftello function or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_get_offset(
     libcfile_stream_t *stream,
     off64_t *offset,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_get_offset";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
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
#if defined( WINAPI )
	*offset = (off64_t) ftell(
	                     internal_stream->stream );
#elif defined( HAVE_FTELLO64 ) && !defined( HAVE_FTELLO )
	*offset = ftello64(
	           internal_stream->stream );
#else
	*offset = (off64_t) ftello(
	                     internal_stream->stream );
#endif
	if( *offset < 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 errno,
		 "%s: unable to get offset from stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#else
#error Missing file stream ftello function
#endif

#if ( defined( HAVE_FSTAT ) && defined( HAVE_FILENO ) && ( defined( HAVE_FSEEKO ) && defined( HAVE_FTELLO ) ) || ( defined( HAVE_FSEEKO64 ) && defined( HAVE_FTELLO64 ) ) ) || defined( WINAPI )

/* Retrieves the size of the file
 * This function uses the POSIX fstat function or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_get_size(
     libcfile_stream_t *stream,
     size64_t *size,
     libcerror_error_t **error )
{
#if defined( _MSC_VER )
	struct __stat64 file_statistics;
#elif defined( __BORLANDC__ )
	struct stati64 file_statistics;
#else
	struct stat file_statistics;
#endif

	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_get_size";
	size_t file_statistics_size                 = 0;
	int file_descriptor                         = 0;

#if defined( S_ISBLK ) && defined( S_ISCHR )
	off64_t current_offset                      = 0;
	off64_t offset                              = 0;
#endif

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( _MSC_VER )
	file_statistics_size = sizeof( struct __stat64 );
#elif defined( __BORLANDC__ )
	file_statistics_size = sizeof( struct stati64 );
#else
	file_statistics_size = sizeof( struct stat );
#endif

	if( memory_set(
	     &file_statistics,
	     0,
	     file_statistics_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file statistics.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	file_descriptor = _fileno(
	                   internal_stream->stream );
#else
	file_descriptor = fileno(
	                   internal_stream->stream );
#endif

	if( file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file descriptor of stream.",
		 function );

		return( -1 );
	}
#if defined( _MSC_VER )
	if( _fstat64(
	     file_descriptor,
	     &file_statistics ) != 0 )
#elif defined( __BORLANDC__ )
	if( _fstati64(
	     file_descriptor,
	     &file_statistics ) != 0 )
#else
	if( fstat(
	     file_descriptor,
	     &file_statistics ) != 0 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file statistics.",
		 function );

		return( -1 );
	}
/* TODO implement device support ? */
/* TODO does not work on Mac OS X or WINAPI */
#if defined( S_ISBLK ) && defined( S_ISCHR )
	if( S_ISBLK( file_statistics.st_mode )
	 || S_ISCHR( file_statistics.st_mode ) )
	{
		if( libcfile_stream_get_offset(
		     stream,
		     &current_offset,
		     error ) != 1  )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset.",
			 function );

			return( -1 );
		}
		/* If the file is a device try to seek the end of the file
		 */
		offset = libcfile_stream_seek_offset(
		          stream,
		          0,
		          SEEK_END,
		          error );

		if( offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek end of file.",
			 function );

			return( -1 );
		}
		*size = (size64_t) offset;

		offset = libcfile_stream_seek_offset(
		          stream,
		          current_offset,
		          SEEK_SET,
		          error );

		if( offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: %" PRIi64 ".",
			 function,
			 current_offset );

			return( -1 );
		}
	}
	else
#endif
	{
		*size = (size64_t) file_statistics.st_size;
	}
	return( 1 );
}

#elif ( defined( HAVE_FSEEKO ) && defined( HAVE_FTELLO ) ) || ( defined( HAVE_FSEEKO64 ) && defined( HAVE_FTELLO64 ) ) || defined( WINAPI )

/* Retrieves the size of the file stream
 * This function uses the POSIX fseeko and ftello functions or equivalent
 * Returns 1 if successful or -1 on error
 */
int libcfile_stream_get_size(
     libcfile_stream_t *stream,
     size64_t *size,
     libcerror_error_t **error )
{
	libcfile_internal_stream_t *internal_stream = NULL;
	static char *function                       = "libcfile_stream_get_size";
	off64_t current_offset                      = 0;
	off64_t offset                              = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	internal_stream = (libcfile_internal_stream_t *) stream;

	if( internal_stream->stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid stream - missing stream.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
/* TODO does not work on Mac OS X or WINAPI */
	if( libcfile_stream_get_offset(
	     stream,
	     &current_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to get current offset.",
		 function );

		return( -1 );
	}
	offset = libcfile_stream_seek_offset(
		  stream,
		  0,
		  SEEK_END,
		  error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek end of stream.",
		 function );

		return( -1 );
	}
	*size = (size64_t) offset;

	offset = libcfile_stream_seek_offset(
		  stream,
		  current_offset,
		  SEEK_SET,
		  error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: %" PRIi64 ".",
		 function,
		 current_offset );

		return( -1 );
	}
	return( 1 );
}

#else
#error Missing file stream get size function
#endif

