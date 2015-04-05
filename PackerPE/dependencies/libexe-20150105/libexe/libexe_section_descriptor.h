/*
 * Section descriptor functions
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

#if !defined( _LIBEXE_SECTION_DESCRIPTOR_H )
#define _LIBEXE_SECTION_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libexe_libcerror.h"
#include "libexe_libfcache.h"
#include "libexe_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_section_descriptor libexe_section_descriptor_t;

struct libexe_section_descriptor
{
	/* The name
	 */
	char name[ 9 ];

	/* The name size
	 */
	size_t name_size;

	/* The virtual address
	 */
	uint32_t virtual_address;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;
};

int libexe_section_descriptor_initialize(
     libexe_section_descriptor_t **section_descriptor,
     libcerror_error_t **error );

int libexe_section_descriptor_free(
     libexe_section_descriptor_t **section_descriptor,
     libcerror_error_t **error );

int libexe_section_descriptor_get_data_size(
     libexe_section_descriptor_t *section_descriptor,
     size64_t *data_size,
     libcerror_error_t **error );

int libexe_section_descriptor_get_data_range(
     libexe_section_descriptor_t *section_descriptor,
     off64_t *data_offset,
     size64_t *data_size,
     libcerror_error_t **error );

int libexe_section_descriptor_set_data_range(
     libexe_section_descriptor_t *section_descriptor,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

