/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Mac-10006 <-> Unicode conversion table
 *
 * Copyright 2009 Viktor Szakats <harbour.01 syenar.hu>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbapi.h"
#include "hbapicdp.h"

#define NUMBER_OF_CHARS    256

static USHORT uniCodes[ NUMBER_OF_CHARS ] =
{
   0x0020, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
   0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
   0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8,
   0x2191, 0x2193, 0x2192, 0x2190, 0x2319, 0x2194, 0x25B2, 0x25BC,
   0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
   0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
   0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
   0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
   0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
   0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
   0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
   0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
   0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
   0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
   0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
   0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
   0x00C4, 0x00B9, 0x00B2, 0x00C9, 0x00B3, 0x00D6, 0x00DC, 0x0385,
   0x00E0, 0x00E2, 0x00E4, 0x0384, 0x00A8, 0x00E7, 0x00E9, 0x00E8,
   0x00EA, 0x00EB, 0x00A3, 0x2122, 0x00EE, 0x00EF, 0x2022, 0x00BD,
   0x2030, 0x00F4, 0x00F6, 0x00A6, 0x00AD, 0x00F9, 0x00FB, 0x00FC,
   0x2020, 0x0393, 0x0394, 0x0398, 0x039B, 0x039E, 0x03A0, 0x00DF,
   0x00AE, 0x00A9, 0x03A3, 0x03AA, 0x00A7, 0x2260, 0x00B0, 0x0387,
   0x0391, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x0392, 0x0395, 0x0396,
   0x0397, 0x0399, 0x039A, 0x039C, 0x03A6, 0x03AB, 0x03A8, 0x03A9,
   0x03AC, 0x039D, 0x00AC, 0x039F, 0x03A1, 0x2248, 0x03A4, 0x00AB,
   0x00BB, 0x2026, 0x00A0, 0x03A5, 0x03A7, 0x0386, 0x0388, 0x0153,
   0x2013, 0x2015, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x0389,
   0x038A, 0x038C, 0x038E, 0x03AD, 0x03AE, 0x03AF, 0x03CC, 0x038F,
   0x03CD, 0x03B1, 0x03B2, 0x03C8, 0x03B4, 0x03B5, 0x03C6, 0x03B3,
   0x03B7, 0x03B9, 0x03BE, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF,
   0x03C0, 0x03CE, 0x03C1, 0x03C3, 0x03C4, 0x03B8, 0x03C9, 0x03C2,
   0x03C7, 0x03C5, 0x03B6, 0x03CA, 0x03CB, 0x0390, 0x03B0, 0x0000
};

HB_UNITABLE hb_uniTbl_10006 = { HB_CPID_10006, NUMBER_OF_CHARS, FALSE, uniCodes };
