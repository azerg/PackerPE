/*
 * Debug functions
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
#include <types.h>

#include "libexe_debug.h"
#include "libexe_definitions.h"
#include "libexe_libbfio.h"
#include "libexe_libcerror.h"
#include "libexe_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the COFF header characteristic flags
 */
void libexe_debug_print_file_characteristic_flags(
      uint16_t characteristic_flags )
{
	if( ( characteristic_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0001 (IMAGE_FILE_RELOCS_STRIPPEDD)\n" );
	}
	if( ( characteristic_flags & 0x0002 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0002 (IMAGE_FILE_EXECUTABLE_IMAGED)\n" );
	}
	if( ( characteristic_flags & 0x0004 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0004 (IMAGE_FILE_LINE_NUMS_STRIPPEDD)\n" );
	}
	if( ( characteristic_flags & 0x0008 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0008 (IMAGE_FILE_LOCAL_SYMS_STRIPPEDD)\n" );
	}
	if( ( characteristic_flags & 0x0010 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0010 (IMAGE_FILE_AGGRESSIVE_WS_TRIMD)\n" );
	}
	if( ( characteristic_flags & 0x0020 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0020 (IMAGE_FILE_LARGE_ADDRESS_AWARED)\n" );
	}
	if( ( characteristic_flags & 0x0040 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0040 (IMAGE_FILE_16BIT_MACHINED)\n" );
	}
	if( ( characteristic_flags & 0x0080 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0080 (IMAGE_FILE_BYTES_REVERSED_LOD)\n" );
	}
	if( ( characteristic_flags & 0x0100 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0100 (IMAGE_FILE_32BIT_MACHINED)\n" );
	}
	if( ( characteristic_flags & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0200 (IMAGE_FILE_DEBUG_STRIPPEDD)\n" );
	}
	if( ( characteristic_flags & 0x0400 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0400 (IMAGE_FILE_REMOVABLE_RUN_FROM_SWAPD)\n" );
	}

	if( ( characteristic_flags & 0x1000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x1000 (IMAGE_FILE_SYSTEMD)\n" );
	}
	if( ( characteristic_flags & 0x2000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x2000 (IMAGE_FILE_DLLD)\n" );
	}
	if( ( characteristic_flags & 0x4000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x4000 (IMAGE_FILE_UP_SYSTEM_ONLYD)\n" );
	}
	if( ( characteristic_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x8000 (IMAGE_FILE_BYTES_REVERSED_HID)\n" );
	}
}

/* Prints the DLL characteristic flags
 */
void libexe_debug_print_dll_characteristic_flags(
      uint16_t characteristic_flags )
{
	if( ( characteristic_flags & 0x0800 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0800 (IMAGE_DLLCHARACTERISTICS_NO_BIND)\n" );
	}

	if( ( characteristic_flags & 0x2000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x2000 (IMAGE_DLLCHARACTERISTICS_WDM_DRIVER)\n" );
	}

	if( ( characteristic_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x8000 (IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)\n" );
	}
}

/* Prints the section characteristic flags
 */
void libexe_debug_print_section_characteristic_flags(
      uint32_t characteristic_flags )
{
	if( characteristic_flags == 0x00000000UL )
	{
		libcnotify_printf(
		 "\t0x00000000 (IMAGE_SCN_TYPE_REG)\n" );
	}
	if( ( characteristic_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000001 (IMAGE_SCN_TYPE_DSECT)\n" );
	}
	if( ( characteristic_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000002 (IMAGE_SCN_TYPE_NOLOAD)\n" );
	}
	if( ( characteristic_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000004 (IMAGE_SCN_TYPE_GROUP)\n" );
	}
	if( ( characteristic_flags & 0x00000008UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000008 (IMAGE_SCN_TYPE_NO_PAD)\n" );
	}
	if( ( characteristic_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000010 (IMAGE_SCN_TYPE_COPY)\n" );
	}
	if( ( characteristic_flags & 0x00000020UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000020 (IMAGE_SCN_CNT_CODE)\n" );
	}
	if( ( characteristic_flags & 0x00000040UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000040 (IMAGE_SCN_CNT_INITIALIZED_DATA)\n" );
	}
	if( ( characteristic_flags & 0x00000080UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000080 (IMAGE_SCN_CNT_UNINITIALIZED_DATA)\n" );
	}
	if( ( characteristic_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000100 (IMAGE_SCN_LNK_OTHER)\n" );
	}
	if( ( characteristic_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000200 (IMAGE_SCN_LNK_INFO)\n" );
	}
	if( ( characteristic_flags & 0x00000400UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000400 (IMAGE_SCN_TYPE_OVER)\n" );
	}
	if( ( characteristic_flags & 0x00000800UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00000800 (IMAGE_SCN_LNK_REMOVE)\n" );
	}
	if( ( characteristic_flags & 0x00001000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00001000 (IMAGE_SCN_LNK_COMDAT)\n" );
	}

	if( ( characteristic_flags & 0x00008000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00008000 (IMAGE_SCN_MEM_FARDATA)\n" );
	}
	if( ( characteristic_flags & 0x00010000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00010000 (IMAGE_SCN_MEM_PURGEABLE)\n" );
	}
	if( ( characteristic_flags & 0x00020000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00020000 (IMAGE_SCN_MEM_16BIT)\n" );
	}
	if( ( characteristic_flags & 0x00040000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00040000 (IMAGE_SCN_MEM_LOCKED)\n" );
	}
	if( ( characteristic_flags & 0x00080000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x00080000 (IMAGE_SCN_MEM_PRELOAD)\n" );
	}
	switch( characteristic_flags & 0x00f00000UL )
	{
		case 0x00100000UL:
			libcnotify_printf(
			 "\t0x00100000 (IMAGE_SCN_ALIGN_1BYTES)\n" );
			break;

		case 0x00200000UL:
			libcnotify_printf(
			 "\t0x00200000 (IMAGE_SCN_ALIGN_2BYTES)\n" );
			break;

		case 0x00300000UL:
			libcnotify_printf(
			 "\t0x00300000 (IMAGE_SCN_ALIGN_4BYTES)\n" );
			break;

		case 0x00400000UL:
			libcnotify_printf(
			 "\t0x00400000 (IMAGE_SCN_ALIGN_8BYTES)\n" );
			break;

		case 0x00500000UL:
			libcnotify_printf(
			 "\t0x00500000 (IMAGE_SCN_ALIGN_16BYTES)\n" );
			break;

		case 0x00600000UL:
			libcnotify_printf(
			 "\t0x00600000 (IMAGE_SCN_ALIGN_32BYTES)\n" );
			break;

		case 0x00700000UL:
			libcnotify_printf(
			 "\t0x00700000 (IMAGE_SCN_ALIGN_64BYTES)\n" );
			break;

		case 0x00800000UL:
			libcnotify_printf(
			 "\t0x00800000 (IMAGE_SCN_ALIGN_128BYTES)\n" );
			break;

		case 0x00900000UL:
			libcnotify_printf(
			 "\t0x00900000 (IMAGE_SCN_ALIGN_256BYTES)\n" );
			break;

		case 0x00a00000UL:
			libcnotify_printf(
			 "\t0x00a00000 (IMAGE_SCN_ALIGN_512BYTES)\n" );
			break;

		case 0x00b00000UL:
			libcnotify_printf(
			 "\t0x00b00000 (IMAGE_SCN_ALIGN_1024BYTES)\n" );
			break;

		case 0x00c00000UL:
			libcnotify_printf(
			 "\t0x00c00000 (IMAGE_SCN_ALIGN_2048BYTES)\n" );
			break;

		case 0x00d00000UL:
			libcnotify_printf(
			 "\t0x00d00000 (IMAGE_SCN_ALIGN_4096BYTES)\n" );
			break;

		case 0x00e00000UL:
			libcnotify_printf(
			 "\t0x00e00000 (IMAGE_SCN_ALIGN_8192BYTES)\n" );
			break;

		default:
			break;
	}

	if( ( characteristic_flags & 0x01000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x01000000 (IMAGE_SCN_LNK_NRELOC_OVFL)\n" );
	}
	if( ( characteristic_flags & 0x02000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x02000000 (IMAGE_SCN_MEM_DISCARDABLE)\n" );
	}
	if( ( characteristic_flags & 0x04000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x04000000 (IMAGE_SCN_MEM_NOT_CACHED)\n" );
	}
	if( ( characteristic_flags & 0x08000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x08000000 (IMAGE_SCN_MEM_NOT_PAGED)\n" );
	}
	if( ( characteristic_flags & 0x10000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x10000000 (IMAGE_SCN_MEM_SHARED)\n" );
	}
	if( ( characteristic_flags & 0x20000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x20000000 (IMAGE_SCN_MEM_EXECUTE)\n" );
	}
	if( ( characteristic_flags & 0x40000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x40000000 (IMAGE_SCN_MEM_READ)\n" );
	}
	if( ( characteristic_flags & 0x80000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t0x80000000 (IMAGE_SCN_MEM_WRITE)\n" );
	}
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libexe_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libexe_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

