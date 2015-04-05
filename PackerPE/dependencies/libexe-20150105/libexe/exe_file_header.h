/*
 * The file header definition of an executable (EXE) file
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

#if !defined( _EXE_FILE_HEADER_H )
#define _EXE_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct exe_mz_header exe_mz_header_t;

struct exe_mz_header
{
	/* Signature
	 * Consists of 2 bytes
	 * "MZ"
	 */
	uint8_t signature[ 2 ];

	/* The size of the last page
	 * Consists of 2 bytes
	 */
	uint8_t last_page_size[ 2 ];

	/* The number of pages
	 * Consists of 2 bytes
	 */
	uint8_t number_of_pages[ 2 ];

	/* The number of relocation entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_relocation_entries[ 2 ];

	/* The number of header paragraphs
	 * Consists of 2 bytes
	 */
	uint8_t number_of_header_paragraphs[ 2 ];

	/* The minimum number of allocated paragraphs
	 * Consists of 2 bytes
	 */
	uint8_t minimum_allocated_paragraphs[ 2 ];

	/* The maximum number of allocated paragraphs
	 * Consists of 2 bytes
	 */
	uint8_t maximum_allocated_paragraphs[ 2 ];

	/* The initial stack segment
	 * Consists of 2 bytes
	 */
	uint8_t initial_stack_segment[ 2 ];

	/* The initial stack pointer
	 * Consists of 2 bytes
	 */
	uint8_t initial_stack_pointer[ 2 ];

	/* The checksum
	 * Consists of 2 bytes
	 */
	uint8_t checksum[ 2 ];

	/* The entry point
	 * Consists of 4 bytes
	 */
	uint8_t entry_point[ 4 ];

	/* The relocation table offset
	 * Consists of 2 bytes
	 */
	uint8_t relocation_table_offset[ 2 ];

	/* The overlay number
	 * Consists of 2 bytes
	 */
	uint8_t overlay_number[ 2 ];

	/* Unknown
	 * Consists of 32 bytes
	 */
	uint8_t unknown1[ 32 ];

	/* The extended header offset
	 * Consists of 4 bytes
	 */
	uint8_t extended_header_offset[ 4 ];

	/* Unknown
	 * Consists of 112 bytes
	 */
	uint8_t unknown2[ 112 ];
};

typedef struct exe_le_header exe_le_header_t;

struct exe_le_header
{
	/* Signature
	 * Consists of 2 bytes
	 * "LE"
	 */
	uint8_t signature[ 2 ];

/* TODO */
};

typedef struct exe_ne_header exe_ne_header_t;

struct exe_ne_header
{
	/* Signature
	 * Consists of 2 bytes
	 * "NE"
	 */
	uint8_t signature[ 2 ];

/* TODO */
	/* Todo
	 * Consists of 62 bytes
	 */
	uint8_t todo[ 62 ];
};

typedef struct exe_pe_header exe_pe_header_t;

struct exe_pe_header
{
	/* Signature
	 * Consists of 4 bytes
	 * "PE\x0\x0"
	 */
	uint8_t signature[ 4 ];
};

typedef struct exe_coff_header exe_coff_header_t;

struct exe_coff_header
{
	/* The target architecture type
	 * Consists of 2 bytes
	 */
	uint8_t target_architecture_type[ 2 ];

	/* The number of sections
	 * Consists of 2 bytes
	 */
	uint8_t number_of_sections[ 2 ];

	/* The number of sections
	 * Consists of 4 bytes
	 * Contains a POSIX timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The symbol table offset
	 * Consists of 4 bytes
	 */
	uint8_t symbol_table_offset[ 4 ];

	/* The number of symbols
	 * Consists of 4 bytes
	 */
	uint8_t number_of_symbols[ 4 ];

	/* The optional header size
	 * Consists of 2 bytes
	 */
	uint8_t optional_header_size[ 2 ];

	/* The characteristic flags
	 * Consists of 2 bytes
	 */
	uint8_t characteristic_flags[ 2 ];
};

typedef struct exe_coff_optional_header exe_coff_optional_header_t;

struct exe_coff_optional_header
{
	/* The signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The major linker version
	 * Consists of 1 byte
	 */
	uint8_t major_linker_version;

	/* The minor linker version
	 * Consists of 1 byte
	 */
	uint8_t minor_linker_version;

	/* The text section size
	 * Consists of 4 bytes
	 */
	uint8_t text_section_size[ 4 ];

	/* The initialized data section size
	 * Consists of 4 bytes
	 */
	uint8_t initialized_data_section_size[ 4 ];

	/* The uninitialized data section size
	 * Consists of 4 bytes
	 */
	uint8_t uninitialized_data_section_size[ 4 ];

	/* The entry point offset
	 * Consists of 4 bytes
	 */
	uint8_t entry_point_offset[ 4 ];

	/* The code base offset
	 * Consists of 4 bytes
	 */
	uint8_t code_base_offset[ 4 ];

	/* The data base offset
	 * Consists of 4 bytes
	 */
	uint8_t data_base_offset[ 4 ];
};

typedef struct exe_coff_optional_header_pe32 exe_coff_optional_header_pe32_t;

struct exe_coff_optional_header_pe32
{
	/* The image base offset
	 * Consists of 4 bytes
	 */
	uint8_t image_base_offset[ 4 ];

	/* The section alignment size
	 * Consists of 4 bytes
	 */
	uint8_t section_alignment_size[ 4 ];

	/* The file alignment size
	 * Consists of 4 bytes
	 */
	uint8_t file_alignment_size[ 4 ];

	/* The major operating system version
	 * Consists of 2 bytes
	 */
	uint8_t major_operating_system_version[ 2 ];

	/* The minor operating system version
	 * Consists of 2 bytes
	 */
	uint8_t minor_operating_system_version[ 2 ];

	/* The major image version
	 * Consists of 2 bytes
	 */
	uint8_t major_image_version[ 2 ];

	/* The minor image version
	 * Consists of 2 bytes
	 */
	uint8_t minor_image_version[ 2 ];

	/* The major subsystem version
	 * Consists of 2 bytes
	 */
	uint8_t major_subsystem_version[ 2 ];

	/* The minor subsystem version
	 * Consists of 2 bytes
	 */
	uint8_t minor_subsystem_version[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The image size
	 * Consists of 4 bytes
	 */
	uint8_t image_size[ 4 ];

	/* The headers size
	 * Consists of 4 bytes
	 */
	uint8_t headers_size[ 4 ];

	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The subsystem
	 * Consists of 2 bytes
	 */
	uint8_t subsystem[ 2 ];

	/* The DLL characteristic flags
	 * Consists of 2 bytes
	 */
	uint8_t dll_characteristic_flags[ 2 ];

	/* The stack reservation size
	 * Consists of 4 bytes
	 */
	uint8_t stack_reservation_size[ 4 ];

	/* The stack commit size
	 * Consists of 4 bytes
	 */
	uint8_t stack_commit_size[ 4 ];

	/* The heap reservation size
	 * Consists of 4 bytes
	 */
	uint8_t heap_reservation_size[ 4 ];

	/* The heap commit size
	 * Consists of 4 bytes
	 */
	uint8_t heap_commit_size[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The number of data-directories entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_directories_entries[ 4 ];
};

typedef struct exe_coff_optional_header_pe32_plus exe_coff_optional_header_pe32_plus_t;

struct exe_coff_optional_header_pe32_plus
{
	/* The image base offset
	 * Consists of 8 bytes
	 */
	uint8_t image_base_offset[ 8 ];

	/* The section alignment size
	 * Consists of 4 bytes
	 */
	uint8_t section_alignment_size[ 4 ];

	/* The file alignment size
	 * Consists of 4 bytes
	 */
	uint8_t file_alignment_size[ 4 ];

	/* The major operating system version
	 * Consists of 2 bytes
	 */
	uint8_t major_operating_system_version[ 2 ];

	/* The minor operating system version
	 * Consists of 2 bytes
	 */
	uint8_t minor_operating_system_version[ 2 ];

	/* The major image version
	 * Consists of 2 bytes
	 */
	uint8_t major_image_version[ 2 ];

	/* The minor image version
	 * Consists of 2 bytes
	 */
	uint8_t minor_image_version[ 2 ];

	/* The major subsystem version
	 * Consists of 2 bytes
	 */
	uint8_t major_subsystem_version[ 2 ];

	/* The minor subsystem version
	 * Consists of 2 bytes
	 */
	uint8_t minor_subsystem_version[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The image size
	 * Consists of 4 bytes
	 */
	uint8_t image_size[ 4 ];

	/* The headers size
	 * Consists of 4 bytes
	 */
	uint8_t headers_size[ 4 ];

	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The subsystem
	 * Consists of 2 bytes
	 */
	uint8_t subsystem[ 2 ];

	/* The DLL characteristic flags
	 * Consists of 2 bytes
	 */
	uint8_t dll_characteristic_flags[ 2 ];

	/* The stack reservation size
	 * Consists of 8 bytes
	 */
	uint8_t stack_reservation_size[ 8 ];

	/* The stack commit size
	 * Consists of 8 bytes
	 */
	uint8_t stack_commit_size[ 8 ];

	/* The heap reservation size
	 * Consists of 8 bytes
	 */
	uint8_t heap_reservation_size[ 8 ];

	/* The heap commit size
	 * Consists of 8 bytes
	 */
	uint8_t heap_commit_size[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The number of data-directories entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_directories_entries[ 4 ];
};

typedef struct exe_coff_optional_header_data_directories exe_coff_optional_header_data_directories_t;

struct exe_coff_optional_header_data_directories
{
	/* The export table RVA
	 * Consists of 4 bytes
	 */
	uint8_t export_table_rva[ 4 ];

	/* The export table size
	 * Consists of 4 bytes
	 */
	uint8_t export_table_size[ 4 ];

	/* The import table RVA
	 * Consists of 4 bytes
	 */
	uint8_t import_table_rva[ 4 ];

	/* The import table size
	 * Consists of 4 bytes
	 */
	uint8_t import_table_size[ 4 ];

	/* The resource table RVA
	 * Consists of 4 bytes
	 */
	uint8_t resource_table_rva[ 4 ];

	/* The resource table size
	 * Consists of 4 bytes
	 */
	uint8_t resource_table_size[ 4 ];

	/* The exception table RVA
	 * Consists of 4 bytes
	 */
	uint8_t exception_table_rva[ 4 ];

	/* The exception table size
	 * Consists of 4 bytes
	 */
	uint8_t exception_table_size[ 4 ];

	/* The certificate table RVA
	 * Consists of 4 bytes
	 */
	uint8_t certificate_table_rva[ 4 ];

	/* The certificate table size
	 * Consists of 4 bytes
	 */
	uint8_t certificate_table_size[ 4 ];

	/* The base relocation table RVA
	 * Consists of 4 bytes
	 */
	uint8_t base_relocation_table_rva[ 4 ];

	/* The base relocation table size
	 * Consists of 4 bytes
	 */
	uint8_t base_relocation_table_size[ 4 ];

	/* The debug data
	 * Consists of 4 bytes
	 */
	uint8_t debug_data_rva[ 4 ];

	/* The debug data
	 * Consists of 4 bytes
	 */
	uint8_t debug_data_size[ 4 ];

	/* The architecture-specific data
	 * Consists of 4 bytes
	 */
	uint8_t architecture_specific_data_rva[ 4 ];

	/* The architecture-specific data
	 * Consists of 4 bytes
	 */
	uint8_t architecture_specific_data_size[ 4 ];

	/* The global pointer register
	 * Consists of 4 bytes
	 */
	uint8_t global_pointer_register[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* The thread local storage table RVA
	 * Consists of 4 bytes
	 */
	uint8_t thread_local_storage_table_rva[ 4 ];

	/* The thread local storage table size
	 * Consists of 4 bytes
	 */
	uint8_t thread_local_storage_table_size[ 4 ];

	/* The load configuration table RVA
	 * Consists of 4 bytes
	 */
	uint8_t load_configuration_table_rva[ 4 ];

	/* The load configuration table size
	 * Consists of 4 bytes
	 */
	uint8_t load_configuration_table_size[ 4 ];

	/* The bound import table RVA
	 * Consists of 4 bytes
	 */
	uint8_t bound_import_table_rva[ 4 ];

	/* The bound import table size
	 * Consists of 4 bytes
	 */
	uint8_t bound_import_table_size[ 4 ];

	/* The import address table RVA
	 * Consists of 4 bytes
	 */
	uint8_t import_address_table_rva[ 4 ];

	/* The import address table size
	 * Consists of 4 bytes
	 */
	uint8_t import_address_table_size[ 4 ];

	/* The delay import descriptor RVA
	 * Consists of 4 bytes
	 */
	uint8_t delay_import_descriptor_rva[ 4 ];

	/* The delay import descriptor size
	 * Consists of 4 bytes
	 */
	uint8_t delay_import_descriptor_size[ 4 ];

	/* The COM+ runtime header RVA
	 * Consists of 4 bytes
	 */
	uint8_t com_plus_runtime_header_rva[ 4 ];

	/* The COM+ runtime header size
	 * Consists of 4 bytes
	 */
	uint8_t com_plus_runtime_header_size[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif

