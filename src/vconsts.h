/*      vconsts.h
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
          VMTrue,
          VMFalse,
          VMNull,
          VMNotFound,
          VMPtr,
          VMInteger,
          VMHeaderSize,
// version ID
          VMMajorVersion,
          VMMinorVersion,
// settings
          MIN_MEM,
          MIN_SEGS,
          MEM_PAD,
          LN_TOKS;

#endif
