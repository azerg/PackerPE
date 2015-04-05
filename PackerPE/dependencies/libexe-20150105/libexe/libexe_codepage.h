/*
 * Codepage functions
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

#if !defined( _LIBEXE_INTERNAL_CODEPAGE_H )
#define _LIBEXE_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEXE )

#include <libexe/codepage.h>

/* Define HAVE_LOCAL_LIBEXE for local use of libexe
 * The definitions in <libexe/codepage.h> are copied here
 * for local use of libexe
 */
#else

/* The codepage definitions
 */
enum LIBEXE_CODEPAGES
{
	LIBEXE_CODEPAGE_ASCII				= 20127,

	LIBEXE_CODEPAGE_ISO_8859_1			= 28591,
	LIBEXE_CODEPAGE_ISO_8859_2			= 28592,
	LIBEXE_CODEPAGE_ISO_8859_3			= 28593,
	LIBEXE_CODEPAGE_ISO_8859_4			= 28594,
	LIBEXE_CODEPAGE_ISO_8859_5			= 28595,
	LIBEXE_CODEPAGE_ISO_8859_6			= 28596,
	LIBEXE_CODEPAGE_ISO_8859_7			= 28597,
	LIBEXE_CODEPAGE_ISO_8859_8			= 28598,
	LIBEXE_CODEPAGE_ISO_8859_9			= 28599,
	LIBEXE_CODEPAGE_ISO_8859_10			= 28600,
	LIBEXE_CODEPAGE_ISO_8859_11			= 28601,
	LIBEXE_CODEPAGE_ISO_8859_13			= 28603,
	LIBEXE_CODEPAGE_ISO_8859_14			= 28604,
	LIBEXE_CODEPAGE_ISO_8859_15			= 28605,
	LIBEXE_CODEPAGE_ISO_8859_16			= 28606,

	LIBEXE_CODEPAGE_KOI8_R				= 20866,
	LIBEXE_CODEPAGE_KOI8_U				= 21866,

	LIBEXE_CODEPAGE_WINDOWS_874			= 874,
	LIBEXE_CODEPAGE_WINDOWS_932			= 932,
	LIBEXE_CODEPAGE_WINDOWS_936			= 936,
	LIBEXE_CODEPAGE_WINDOWS_949			= 949,
	LIBEXE_CODEPAGE_WINDOWS_950			= 950,
	LIBEXE_CODEPAGE_WINDOWS_1250			= 1250,
	LIBEXE_CODEPAGE_WINDOWS_1251			= 1251,
	LIBEXE_CODEPAGE_WINDOWS_1252			= 1252,
	LIBEXE_CODEPAGE_WINDOWS_1253			= 1253,
	LIBEXE_CODEPAGE_WINDOWS_1254			= 1254,
	LIBEXE_CODEPAGE_WINDOWS_1255			= 1255,
	LIBEXE_CODEPAGE_WINDOWS_1256			= 1256,
	LIBEXE_CODEPAGE_WINDOWS_1257			= 1257,
	LIBEXE_CODEPAGE_WINDOWS_1258			= 1258
};

#endif

#if defined( __cplusplus )
}
#endif

#endif

