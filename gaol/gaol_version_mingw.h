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
 * CVS: $Id: gaol_version_msvc.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_version_mingw.h
  \brief  Version information when using MinGW

  <long description>

  \author Frederic Goualard
  \date   2006-03-22
*/


#ifndef __gaol_version_mingw_h__
#define __gaol_version_mingw_h__

#define GAOL_MAJOR_VERSION 4
#define GAOL_MINOR_VERSION 2
#define GAOL_MICRO_VERSION 3
// Defining GAOL_VERSION requires some stringification tricks
#define GAOL_VERSION GAOL_VERSION1(GAOL_MAJOR,GAOL_MINOR,GAOL_MICRO)
#define GAOL_VERSION1(A,B,C) GAOL_VERSION2(A,B,C)
#define GAOL_VERSION2(A,B,C) #A "." #B "." #C

#endif /* __gaol_version_mingw_h__ */
