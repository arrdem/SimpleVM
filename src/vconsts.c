#ifndef _vconsts_c_
#define _vconsts_c_

const int VMTrue      = 1,
          VMFalse     = 0,
          VMNull      = 0xDEADBEEF,
          VMNotFound  = 0xFEEDFACE,
          MIN_MEM     = 15,
          MIN_SEGS    = 15;
          MEM_PAD     = 2,
          VMPtr       = 0,
          VMInteger   = 0xAAAAAAAA,
          VMBoolean   = 0xBBBBBBBB,
          VMArray     = 0xCCCCCCCC,
          VMStack     = 0xDDDDDDDD;

#endif
