/*
 * Import table functions
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

#if !defined( _LIBEXE_IMPORT_TABLE_H )
#define _LIBEXE_IMPORT_TABLE_H

#include <common.h>
#include <types.h>

#include "libexe_libbfio.h"
#include "libexe_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_import_table libexe_import_table_t;

struct libexe_import_table
{
	/* Dummy
	 */
	int dummy;
};

int libexe_import_table_initialize(
     libexe_import_table_t **import_table,
     libcerror_error_t **error );

int libexe_import_table_free(
     libexe_import_table_t **import_table,
     libcerror_error_t **error );

int libexe_import_table_read(
     libexe_import_table_t *import_table,
     libbfio_handle_t *file_io_handle,
     uint32_t file_offset,
     uint32_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

