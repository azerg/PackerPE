/*
 * The section table definition of an executable (EXE) file
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

#if !defined( _EXE_SECTION_TABLE_H )
#define _EXE_SECTION_TABLE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct exe_section_table_entry exe_section_table_entry_t;

struct exe_section_table_entry
{
	/* The name
	 * Consists of 8 bytes
	 * Contains an ASCII string
	 */
	uint8_t name[ 8 ];

	/* The virtual size
	 * Consists of 4 bytes
	 */
	uint8_t virtual_size[ 4 ];

	/* The virtual address
	 * Consists of 4 bytes
	 */
	uint8_t virtual_address[ 4 ];

	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];

	/* The data offset
	 * Consists of 4 bytes
	 */
	uint8_t data_offset[ 4 ];

	/* The relocations offset
	 * Consists of 4 bytes
	 */
	uint8_t relocations_offset[ 4 ];

	/* The line numbers offset
	 * Consists of 4 bytes
	 */
	uint8_t line_numbers_offset[ 4 ];

	/* The number of relocations
	 * Consists of 2 bytes
	 */
	uint8_t number_of_relocations[ 2 ];

	/* The number of line numbers
	 * Consists of 2 bytes
	 */
	uint8_t number_of_line_numbers[ 2 ];

	/* The section characteristic flags
	 * Consists of 4 bytes
	 */
	uint8_t section_characteristic_flags[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif

