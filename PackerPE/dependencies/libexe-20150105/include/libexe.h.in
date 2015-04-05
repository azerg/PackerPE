/*
 * Library to access the executable (EXE) format
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

#if !defined( _LIBEXE_H )
#define _LIBEXE_H

#include <libexe/codepage.h>
#include <libexe/definitions.h>
#include <libexe/error.h>
#include <libexe/extern.h>
#include <libexe/features.h>
#include <libexe/types.h>

#include <stdio.h>

#if defined( LIBEXE_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBEXE_EXTERN \
const char *libexe_get_version(
             void );

/* Returns the access flags for reading
 */
LIBEXE_EXTERN \
int libexe_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_get_codepage(
     int *codepage,
     libexe_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_set_codepage(
     int codepage,
     libexe_error_t **error );

/* Determines if a file contains an EXE file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEXE_EXTERN \
int libexe_check_file_signature(
     const char *filename,
     libexe_error_t **error );

#if defined( LIBEXE_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an EXE file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEXE_EXTERN \
int libexe_check_file_signature_wide(
     const wchar_t *filename,
     libexe_error_t **error );

#endif /* defined( LIBEXE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEXE_HAVE_BFIO )

/* Determines if a file contains an EXE file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEXE_EXTERN \
int libexe_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libexe_error_t **error );

#endif /* defined( LIBEXE_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBEXE_EXTERN \
void libexe_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_notify_set_stream(
     FILE *stream,
     libexe_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_notify_stream_open(
     const char *filename,
     libexe_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_notify_stream_close(
     libexe_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBEXE_EXTERN \
void libexe_error_free(
      libexe_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_error_fprint(
     libexe_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_error_sprint(
     libexe_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_error_backtrace_fprint(
     libexe_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_error_backtrace_sprint(
     libexe_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_initialize(
     libexe_file_t **file,
     libexe_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_free(
     libexe_file_t **file,
     libexe_error_t **error );

/* Signals the libexe file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_signal_abort(
     libexe_file_t *file,
     libexe_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_open(
     libexe_file_t *file,
     const char *filename,
     int access_flags,
     libexe_error_t **error );

#if defined( LIBEXE_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_open_wide(
     libexe_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libexe_error_t **error );

#endif /* defined( LIBEXE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEXE_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_open_file_io_handle(
     libexe_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libexe_error_t **error );

#endif /* defined( LIBEXE_HAVE_BFIO ) */

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_close(
     libexe_file_t *file,
     libexe_error_t **error );

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_get_ascii_codepage(
     libexe_file_t *file,
     int *ascii_codepage,
     libexe_error_t **error );

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_set_ascii_codepage(
     libexe_file_t *file,
     int ascii_codepage,
     libexe_error_t **error );

/* Retrieves the number of sections
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_get_number_of_sections(
     libexe_file_t *file,
     int *number_of_sections,
     libexe_error_t **error );

/* Retrieves a specific section
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_get_section(
     libexe_file_t *file,
     int section_index,
     libexe_section_t **section,
     libexe_error_t **error );

/* Retrieves a specific section by an ASCII formatted name
 * Returns 1 if successful, 0 if no such section or -1 on error
 */
LIBEXE_EXTERN \
int libexe_file_get_section_by_name(
     libexe_file_t *file,
     const char *name,
     size_t name_length,
     libexe_section_t **section,
     libexe_error_t **error );

/* -------------------------------------------------------------------------
 * Section functions
 * ------------------------------------------------------------------------- */

/* Frees a section
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_free(
     libexe_section_t **section,
     libexe_error_t **error );

/* Retrieves the size of the ASCII formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_name_size(
     libexe_section_t *section,
     size_t *name,
     libexe_error_t **error );

/* Retrieves the ASCII formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_name(
     libexe_section_t *section,
     char *name,
     size_t name_size,
     libexe_error_t **error );

/* Retrieves the size of the UTF-8 formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_utf8_name_size(
     libexe_section_t *section,
     size_t *name,
     libexe_error_t **error );

/* Retrieves the UTF-8 formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_utf8_name(
     libexe_section_t *section,
     uint8_t *name,
     size_t name_size,
     libexe_error_t **error );

/* Retrieves the size of the UTF-16 formatted name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_utf16_name_size(
     libexe_section_t *section,
     size_t *name,
     libexe_error_t **error );

/* Retrieves the UTF-16 formatted name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_utf16_name(
     libexe_section_t *section,
     uint16_t *name,
     size_t name_size,
     libexe_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBEXE_EXTERN \
ssize_t libexe_section_read_buffer(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         libexe_error_t **error );

/* Reads data at a specific offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBEXE_EXTERN \
ssize_t libexe_section_read_buffer_at_offset(
         libexe_section_t *section,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libexe_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBEXE_EXTERN \
off64_t libexe_section_seek_offset(
         libexe_section_t *section,
         off64_t offset,
         int whence,
         libexe_error_t **error );

/* Retrieves the (current) offset
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_offset(
     libexe_section_t *section,
     off64_t *offset,
     libexe_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_size(
     libexe_section_t *section,
     size64_t *size,
     libexe_error_t **error );

/* Retrieves the start offset
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_start_offset(
     libexe_section_t *section,
     off64_t *start_offset,
     libexe_error_t **error );

/* Retrieves the virtual address
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_virtual_address(
     libexe_section_t *section,
     uint32_t *virtual_address,
     libexe_error_t **error );

#if defined( LIBEXE_HAVE_BFIO )

/* Retrieves the section data file io handle
 * Returns 1 if successful or -1 on error
 */
LIBEXE_EXTERN \
int libexe_section_get_data_file_io_handle(
     libexe_section_t *section,
     libbfio_handle_t **file_io_handle,
     libexe_error_t **error );

#endif /* defined( LIBEXE_HAVE_BFIO ) */

#if defined( __cplusplus )
}
#endif

#endif

