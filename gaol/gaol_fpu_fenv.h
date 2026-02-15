/*-*-C++-*------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Institut de Recherche
 * en Informatique de Nantes, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2010 Laboratoire d'Informatique de Nantes Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id$
 * By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_fpu_fenv.h
  \brief

  FPU handling through C99 fenv.h facilities

  \author Frédéric Goualard
  \date   2010-04-22
*/

/* FIXME: fesetenv() does not seem to work correctly on Linux at present. 
          It is not a problem as long as proper restriction to 64 bits
          for floatint-point operands has been selected beforehand 
          (e.g., through Mathlib's Init_Lib()).
*/

#ifndef __gaol_fpu_fenv_h__
#define __gaol_fpu_fenv_h__

#include "gaol/gaol_port.h"
#include <fenv.h> // https://sourceforge.net/p/mingw-w64/bugs/541/ ?
#include <string.h>
#if defined(_WIN32)
#  include <float.h>
#  if !defined(_PC_53) || !defined(_MCW_PC)
// Weird problem for MinGW, which sometimes prevents to use here _controlfp(_PC_53, _MCW_PC)...
#undef _PC_64
#define _PC_64 0x00000000
#undef _PC_53
#define _PC_53 0x00010000
#undef _PC_24
#define _PC_24 0x00020000
#undef _MCW_EM
#define _MCW_EM 0x0008001f
#undef _MCW_RC
#define _MCW_RC 0x00000300
#undef _MCW_PC
#define _MCW_PC 0x00030000
#undef _MCW_IC
#define _MCW_IC 0x00040000
#undef _MCW_DN
#define _MCW_DN 0x03000000
#ifdef __cplusplus
extern "C" unsigned int _controlfp(unsigned int, unsigned int);
#else
extern int unsigned int _controlfp(unsigned int, unsigned int);
#endif
#  endif // !defined(_PC_53) || !defined(_MCW_PC)
#endif // defined(_WIN32)

//  Mask 0x0a7f: 53 bits precision, all exceptions masked, rounding to +oo
// FIXME: Using an hexadecimal constant is not portable!
// To improve the portability while keeping performance, please replace all existing use of reset_fpu_cw(GAOL_FPU_MASK) with reset_fpu_cw_to_gaol_defaults()...
#define GAOL_FPU_MASK 0x0a3f

// Warning: even if a control word might exist on almost all platforms, it might
// not be possible to manipulate it the same way...
//
// Also, sometimes it might be larger than an unsigned short...
//
#if defined(__linux__) && (defined(__i386__) || defined(__x86_64__))
#   define CTRLWORD(v) (v).__control_word
#elif defined(__linux__) && defined(__aarch64__)
#   define CTRLWORD(v) (v).__fpcr
#elif defined(__APPLE__) && (defined(__i386__) || defined(__x86_64__))
#   define CTRLWORD(v) (v).__control
#elif defined(__APPLE__) && (defined(__arm64__) || defined(__aarch64__))
#   define CTRLWORD(v) (v).__fpcr
#elif defined(__APPLE__) && defined(__arm__)
#   define CTRLWORD(v) (v).__fpscr
#elif defined(_MSC_VER) || defined(__BORLANDC__) //() && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC))
#   define CTRLWORD(v) (v)._Fe_ctl
#elif defined(__MINGW64_VERSION_MAJOR) && (__MINGW64_VERSION_MAJOR >= 13)
#   define CTRLWORD(v) (v)._Fe_ctl
#elif defined(__MINGW32__) //()  && (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64))
#   define CTRLWORD(v) (v).__control_word
#elif defined(__arm__) || defined(_ARM_)
#   define CTRLWORD(v) (v).__cw
#elif defined(__arm64__) || defined(__aarch64__) || defined(_ARM64_)
#   define CTRLWORD(v) (v).__fpcr
#else
//#   define CTRLWORD(v) (v).__control_word
#   define CTRLWORD(v) (v)
#endif

#if USING_SSE2_INSTRUCTIONS
#  include <xmmintrin.h>
#  if defined(_MSC_VER)
#    include <intrin.h>
#  endif // _MSC_VER
   // Mask for SSE arithmetic (53 bits precision, rounding nearest, all exceptions masked)
#  define GAOL_SSE_MASK _MM_MASK_MASK
#endif


namespace gaol {

INLINE double previous_float(double d)
{
	return nextafter(d,-GAOL_INFINITY);
}

INLINE double next_float(double d)
{
	return nextafter(d,GAOL_INFINITY);
}

#if USING_SSE2_INSTRUCTIONS
	//! Sets rounding direction to -oo for SSE operations only
	INLINE void round_downward_sse(void)
	{
		_mm_setcsr(GAOL_SSE_MASK|_MM_ROUND_DOWN);
	}

	//! Sets rounding direction to the nearest for SSE operations only
	INLINE void round_to_nearest_sse(void)
	{
		_mm_setcsr(GAOL_SSE_MASK|_MM_ROUND_NEAREST);
	}


	//! Sets rounding direction to +oo for SSE operations only
	INLINE void round_upward_sse(void)
	{
		_mm_setcsr(GAOL_SSE_MASK|_MM_ROUND_UP);
	}
#endif // USING_SSE2_INSTRUCTIONS


INLINE  void
round_downward(void)
{
  fesetround(FE_DOWNWARD);
}

INLINE  void
round_upward(void)
{
  fesetround(FE_UPWARD);
}

INLINE  void
round_nearest(void)
{
  fesetround(FE_TONEAREST);
}

INLINE unsigned short int get_fpu_cw()
{
  fenv_t tmp;
  fegetenv(&tmp);
  return (unsigned short int)CTRLWORD(tmp); // Warning: the control word might be larger than an unsigned short (e.g. with Visual Studio).
}

INLINE void reset_fpu_cw(unsigned short int st)
{
  fenv_t tmp;
  fegetenv(&tmp);
#if (defined(__linux__) || defined(__APPLE__)) && (defined(__x86_64__) || defined(__i386__))
  // The control word appears to be always an unsigned short int, probably with the same meaning for each bit...
  CTRLWORD(tmp) = st;
#else
  // Warning: the control word might be larger than an unsigned short (e.g. with Visual Studio).
  // We strongly hope here that this will set the bits related to what was changed before...
  // x87 precision is not controlled with this, but at the moment gaol only changes 
  // the rounding mode when this function is used, so it should be OK...
  memcpy(&CTRLWORD(tmp), &st, sizeof st);
#endif // (defined(__linux__) || defined(__APPLE__)) && (defined(__x86_64__) || defined(__i386__))
  fesetenv(&tmp);
}

// 53 bits precision, all exceptions masked, rounding to +oo.
INLINE void reset_fpu_cw_to_gaol_defaults()
{
  fenv_t tmp;
  feholdexcept(&tmp); // Save the current environment and turn on non-stop floating-point exception handling.
  feclearexcept(FE_ALL_EXCEPT); // Clear all exceptions.
  fesetround(FE_UPWARD); // Rounding to +oo.
// For ARM (and x86_64 Windows, Linux, Mac?), precision of double type is 
// probably always IEEE 754 double and cannot be changed (or possibly only with  compilation flags...?).
// See https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/control87-controlfp-control87-2, 
// http://christian-seiler.de/projekte/fpmath/, ARM C and C++ Libraries and 
// Floating-Point Support User Guide.
# if defined(_WIN32) && (defined(_M_IX86) || defined(__i386__))
  _controlfp(_PC_53, _MCW_PC); // Set x87 precision to 53-bit (double).
# endif // defined(_WIN32) && (defined(_M_IX86) || defined(__i386__))
}

  /*!
    \brief Returns the opposite of the argument

    This macro is used to avoid the optimization if the negation is required
    for trust rounding.
   */
#if GAOL_USING_ASM
#   if IX86_LINUX || IX86_MACOSX || ((defined(__linux__) || defined(__APPLE__)) && (defined(__i386__) || defined(__x86_64__)))
        INLINE double f_negate(double x)
        {
            asm volatile ("fldl %1; fchs; fstpl %0" : "=m" (x) : "m" (x));
            return x;
        }
#   elif defined(_MSC_VER) && defined(_M_IX86) // asm not allowed in 64 bit...
        INLINE double f_negate(double x)
        {
            __asm {
            	fld x
            	fchs
            	fstp x
            }
            return x;
        }
#   else
        INLINE double f_negate(double x)
        {
            uintdouble id;
            id.d = x;
            HI_UINTDOUBLE(id) ^= 0x80000000; // XOR on sign bit
            return id.d;
        }
#   endif
#else
    INLINE double f_negate(double x)
    {
        uintdouble id;
        id.d = x;
        HI_UINTDOUBLE(id) ^= 0x80000000; // XOR on sign bit
        return id.d;
    }
#endif // GAOL_USING_ASM

} // namespace gaol

#endif /* __gaol_fpu_fenv_h__ */
