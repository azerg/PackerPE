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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyexe_file.h"
#include "pyexe_libcerror.h"
#include "pyexe_libexe.h"
#include "pyexe_python.h"
#include "pyexe_section.h"
#include "pyexe_sections.h"

PySequenceMethods pyexe_sections_sequence_methods = {
	/* sq_length */
	(lenfunc) pyexe_sections_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyexe_sections_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyexe_sections_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyexe._sections",
	/* tp_basicsize */
	sizeof( pyexe_sections_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyexe_sections_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pyexe_sections_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyexe sections sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyexe_sections_iter,
	/* tp_iternext */
	(iternextfunc) pyexe_sections_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyexe_sections_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new sections object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyexe_sections_new(
           pyexe_file_t *file_object,
           PyObject* (*get_section_by_index)(
                        pyexe_file_t *file_object,
                        int section_index ),
           int number_of_sections )
{
	pyexe_sections_t *pyexe_sections = NULL;
	static char *function            = "pyexe_sections_new";

	if( file_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file object.",
		 function );

		return( NULL );
	}
	if( get_section_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get section by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the sections values are initialized
	 */
	pyexe_sections = PyObject_New(
	                  struct pyexe_sections,
	                  &pyexe_sections_type_object );

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize sections.",
		 function );

		goto on_error;
	}
	if( pyexe_sections_init(
	     pyexe_sections ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize sections.",
		 function );

		goto on_error;
	}
	pyexe_sections->file_object          = file_object;
	pyexe_sections->get_section_by_index = get_section_by_index;
	pyexe_sections->number_of_sections   = number_of_sections;

	Py_IncRef(
	 (PyObject *) pyexe_sections->file_object );

	return( (PyObject *) pyexe_sections );

on_error:
	if( pyexe_sections != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyexe_sections );
	}
	return( NULL );
}

/* Intializes a sections object
 * Returns 0 if successful or -1 on error
 */
int pyexe_sections_init(
     pyexe_sections_t *pyexe_sections )
{
	static char *function = "pyexe_sections_init";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return( -1 );
	}
	/* Make sure the sections values are initialized
	 */
	pyexe_sections->file_object          = NULL;
	pyexe_sections->get_section_by_index = NULL;
	pyexe_sections->section_index        = 0;
	pyexe_sections->number_of_sections   = 0;

	return( 0 );
}

/* Frees a sections object
 */
void pyexe_sections_free(
      pyexe_sections_t *pyexe_sections )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyexe_sections_free";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyexe_sections );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyexe_sections->file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyexe_sections->file_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyexe_sections );
}

/* The sections len() function
 */
Py_ssize_t pyexe_sections_len(
            pyexe_sections_t *pyexe_sections )
{
	static char *function = "pyexe_sections_len";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyexe_sections->number_of_sections );
}

/* The sections getitem() function
 */
PyObject *pyexe_sections_getitem(
           pyexe_sections_t *pyexe_sections,
           Py_ssize_t item_index )
{
	PyObject *section_object = NULL;
	static char *function    = "pyexe_sections_getitem";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->get_section_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections - missing get section by index function.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->number_of_sections < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections - invalid number of sections.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyexe_sections->number_of_sections ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	section_object = pyexe_sections->get_section_by_index(
	                  pyexe_sections->file_object,
	                  (int) item_index );

	return( section_object );
}

/* The sections iter() function
 */
PyObject *pyexe_sections_iter(
           pyexe_sections_t *pyexe_sections )
{
	static char *function = "pyexe_sections_iter";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyexe_sections );

	return( (PyObject *) pyexe_sections );
}

/* The sections iternext() function
 */
PyObject *pyexe_sections_iternext(
           pyexe_sections_t *pyexe_sections )
{
	PyObject *section_object = NULL;
	static char *function    = "pyexe_sections_iternext";

	if( pyexe_sections == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->get_section_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections - missing get section by index function.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->section_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections - invalid section index.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->number_of_sections < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sections - invalid number of sections.",
		 function );

		return( NULL );
	}
	if( pyexe_sections->section_index >= pyexe_sections->number_of_sections )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	section_object = pyexe_sections->get_section_by_index(
	                  pyexe_sections->file_object,
	                  pyexe_sections->section_index );

	if( section_object != NULL )
	{
		pyexe_sections->section_index++;
	}
	return( section_object );
}

