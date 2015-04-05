/*
 * Python object definition of the sections sequence and iterator
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

#if !defined( _PYEXE_SECTIONS_H )
#define _PYEXE_SECTIONS_H

#include <common.h>
#include <types.h>

#include "pyexe_file.h"
#include "pyexe_libexe.h"
#include "pyexe_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyexe_sections pyexe_sections_t;

struct pyexe_sections
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The pyexe file object
	 */
	pyexe_file_t *file_object;

	/* The get section by index callback function
	 */
	PyObject* (*get_section_by_index)(
	             pyexe_file_t *file_object,
	             int section_index );

	/* The (current) section index
	 */
	int section_index;

	/* The number of sections
	 */
	int number_of_sections;
};

extern PyTypeObject pyexe_sections_type_object;

PyObject *pyexe_sections_new(
           pyexe_file_t *file_object,
           PyObject* (*get_section_by_index)(
                        pyexe_file_t *file_object,
                        int section_index ),
           int number_of_sections );

int pyexe_sections_init(
     pyexe_sections_t *pyexe_sections );

void pyexe_sections_free(
      pyexe_sections_t *pyexe_sections );

Py_ssize_t pyexe_sections_len(
            pyexe_sections_t *pyexe_sections );

PyObject *pyexe_sections_getitem(
           pyexe_sections_t *pyexe_sections,
           Py_ssize_t item_index );

PyObject *pyexe_sections_iter(
           pyexe_sections_t *pyexe_sections );

PyObject *pyexe_sections_iternext(
           pyexe_sections_t *pyexe_sections );

#if defined( __cplusplus )
}
#endif

#endif

