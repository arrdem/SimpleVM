/*      vconsts.c
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      This file defines some magic numbers that I use throughout the
 *      SimpleVM project.
 */

#ifndef _VCONSTS_C_
#define _VCONSTS_C_

const int
// type and basic values
          VMTrue      = 1,
          VMFalse     = 0,
          VMNull      = 0xDEADBEEF,
          VMNotFound  = 0xFEEDFACE,
          VMPtr       = 0,
          VMInteger   = 0xAAAAAAAA,
          VMBoolean   = 0xBBBBBBBB,
          VMArray     = 0xCCCCCCCC,
          VMStack     = 0xDDDDDDDD,
          VMHeaderSize   = 2,
// VM version ID
          VMMajorVersion = 1,
          VMMinorVersion = 7,
// settings
          MIN_MEM     = 15,
          MIN_SEGS    = 15,
          MEM_PAD     = 2,
          LN_TOKS     = 6;
#endif
