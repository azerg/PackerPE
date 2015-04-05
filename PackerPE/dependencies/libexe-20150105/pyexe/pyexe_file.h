/*
 * Python object definition of the libexe file
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

#if !defined( _PYEXE_FILE_H )
#define _PYEXE_FILE_H

#include <common.h>
#include <types.h>

#include "pyexe_libbfio.h"
#include "pyexe_libexe.h"
#include "pyexe_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyexe_file pyexe_file_t;

struct pyexe_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libexe file
	 */
	libexe_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyexe_file_object_methods[];
extern PyTypeObject pyexe_file_type_object;

PyObject *pyexe_file_new(
           void );

PyObject *pyexe_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyexe_file_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyexe_file_init(
     pyexe_file_t *pyexe_file );

void pyexe_file_free(
      pyexe_file_t *pyexe_file );

PyObject *pyexe_file_signal_abort(
           pyexe_file_t *pyexe_file,
           PyObject *arguments );

PyObject *pyexe_file_open(
           pyexe_file_t *pyexe_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyexe_file_open_file_object(
           pyexe_file_t *pyexe_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyexe_file_close(
           pyexe_file_t *pyexe_file,
           PyObject *arguments );

PyObject *pyexe_file_get_ascii_codepage(
           pyexe_file_t *pyexe_file,
           PyObject *arguments );

int pyexe_file_set_ascii_codepage_from_string(
     pyexe_file_t *pyexe_file,
     const char *codepage_string );

PyObject *pyexe_file_set_ascii_codepage(
           pyexe_file_t *pyexe_file,
           PyObject *arguments,
           PyObject *keywords );

int pyexe_file_set_ascii_codepage_setter(
     pyexe_file_t *pyexe_file,
     PyObject *string_object,
     void *closure );

PyObject *pyexe_file_get_number_of_sections(
           pyexe_file_t *pyexe_file,
           PyObject *arguments );

PyObject *pyexe_file_get_section_by_index(
           pyexe_file_t *pyexe_file,
           int section_index );

PyObject *pyexe_file_get_section(
           pyexe_file_t *pyexe_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyexe_file_get_sections(
           pyexe_file_t *pyexe_file,
           PyObject *arguments );

PyObject *pyexe_file_get_section_by_name(
           pyexe_file_t *pyexe_file,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif

