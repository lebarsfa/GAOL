/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Laboratoire d'Informatique de
 * Nantes-Atlantique, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2006 Laboratoire d'Informatique de
 *                         Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: gaol_config.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_config.h
  \brief  Configuration file

  This file contains the declarations of all the macros used to control the
  way gaol is compiled, depending on the host. It acts as a front-end for
  the configuration file created by configure and the one used when
  compiling gaol with Borland C++ Builder.


  \author Frederic Goualard
  \date   2002-12-03
*/

#ifndef __gaol_config_h__
#define __gaol_config_h__

#if defined (_MSC_VER)

# define GAOL_ERRNO errno
// No shared library support a.t.m.
# ifndef __GAOL_PUBLIC__
#   ifdef _COMPILING__GAOL_PUBLIC__
//#     define __GAOL_PUBLIC__ __declspec(dllexport)
#     define __GAOL_PUBLIC__
#   else
//#     define __GAOL_PUBLIC__ __declspec(dllimport)
#     define __GAOL_PUBLIC__
#   endif
# endif
# define INLINE inline
# ifdef DISABLE_GAOL_CONFIGURATION_H
#  include "gaol/gaol_config_msvc.h"
# else
#  include "gaol/gaol_configuration.h"
#endif

//#elif defined (__MINGW32__)
//
//# define GAOL_ERRNO errno
//# undef PACKAGE
//# undef VERSION
//# ifdef DISABLE_GAOL_CONFIGURATION_H
//#  include "gaol/gaol_config_mingw.h"
//# else
//#  include "gaol/gaol_configuration.h"
//# endif
//# ifndef __GAOL_PUBLIC__
//#  if defined (HAVE_VISIBILITY_OPTIONS)
//#     define __GAOL_PUBLIC__ __attribute__ ((visibility("default")))
//#  else
//#     define __GAOL_PUBLIC__
//#  endif
//# endif
//# define INLINE inline
//
#elif defined (__GNUC__)

# define GAOL_ERRNO errno
# undef PACKAGE
# undef VERSION
# include "gaol/gaol_configuration.h"
# ifndef __GAOL_PUBLIC__
#  if defined (HAVE_VISIBILITY_OPTIONS)
#     define __GAOL_PUBLIC__ __attribute__ ((visibility("default")))
#  else
#     define __GAOL_PUBLIC__
#  endif
# endif
# define INLINE inline

#else

# define GAOL_ERRNO errno
# undef PACKAGE
# undef VERSION
# ifndef __GAOL_PUBLIC__
#  define __GAOL_PUBLIC__
# endif
# define INLINE inline
# include "gaol/gaol_configuration.h"

#endif

/* Try to be more independent from autotools to simplify compatibility with other build systems */

// For Visual Studio, the flags /fp:strict and /Zc:strictStrings- might be necessary. Adding a 
// partial implementation of unistd.h (e.g. from https://github.com/ENSTABretagneRobotics/OSUtils/blob/master/To%20add%20or%20modify%20if%20needed/VS/unistd.h)
// and /Zc:__cplusplus might increase portability.
// Depending on how the code is built, note that gaol_interval_fpu.cpp and 
// gaol_interval_sse.cpp should not be added to the project, and /D __GAOL_PUBLIC__= might be also useful.
// gaol_interval2f.cpp should not be added to the project if !USING_SSE3_INSTRUCTIONS.

#if defined(_MSC_VER)
#   pragma fenv_access(on)
#else
// Does not seem to exist in C++...
// See https://stackoverflow.com/questions/36218367/does-fenv-access-pragma-exist-in-c11-and-higher
//#   pragma STDC FENV_ACCESS ON
#endif

// See https://devblogs.microsoft.com/cppblog/c99-library-support-in-visual-studio-2013/ 
// for information about new headers to get Visual Studio closer to C99...
// See also https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus.
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1800))
#   ifndef HAVE_FENV_H
#      define HAVE_FENV_H 1
#   endif
#   ifndef HAVE_ISNAN
#      define HAVE_ISNAN 1
#   endif
#   ifndef HAVE_NEXTAFTER
#      define HAVE_NEXTAFTER 1
#   endif
#endif // (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1800))

#undef HAVE__NEXTAFTER
#if defined(_MSC_VER)
#   define HAVE__NEXTAFTER 1
#endif // defined(_MSC_VER)

#if (__cplusplus >= 199711L) || defined(_MSC_VER)
#   ifndef HAVE_CLOCK
#      define HAVE_CLOCK 1
#   endif
#   ifndef CLOCK_IN_HEADER
#      define CLOCK_IN_HEADER 1
#   endif
#   ifndef HAVE___SIGNBIT
#      define HAVE___SIGNBIT 1
#   endif
#   ifndef HAVE_LIMITS
#      define HAVE_LIMITS 1
#   endif
#endif

#endif /* __gaol_config_h__ */
