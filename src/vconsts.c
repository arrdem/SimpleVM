/*      vmconsts.c
 *
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License as
 *      published by the Free Software Foundation; either version 2 of
 *      the License, or (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public
 *      License along with this program; if not, write to the Free
 *      Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *      Boston, MA 02110-1301, USA.
 *
 *      This file defines some magic numbers that I use throughout the
 *      SimpleVM project.
 */

#ifndef _vconsts_c_
#define _vconsts_c_

const int
// System settings....
          MIN_MEM     = 15,
          MIN_SEGS    = 15,
          MEM_PAD     = 2,
          LN_TOKS     = 6,

// System values
//// data types
          VMPtr       = 0,
          VMInteger   = 0xAAAAAAAA,
          VMBoolean   = 0xBBBBBBBB,
          VMArray     = 0xCCCCCCCC,
          VMStack     = 0xDDDDDDDD,

//// data values
          VMTrue      = 1,
          VMFalse     = 0,
          VMNull      = 0xDEADBEEF,
          VMNotFound  = 0xFEEDFACE,

//// parser constants
          VM_SPACE    = 0x10000000,
          VM_STR      = 0x02000000,
          VM_INTEGER  = 0x00300000,
          VM_EOL      = 0x12345678;

#endif
