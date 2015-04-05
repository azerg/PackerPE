/*
 * Data directory descriptor functions
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

#if !defined( _LIBEXE_DATA_DIRECTORY_DESCRIPTOR_H )
#define _LIBEXE_DATA_DIRECTORY_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libexe_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libexe_data_directory_descriptor libexe_data_directory_descriptor_t;

struct libexe_data_directory_descriptor
{
	/* The virtual address
	 */
	uint32_t virtual_address;

	/* The size
	 */
	uint32_t size;
};

#if defined( __cplusplus )
}
#endif

#endif

