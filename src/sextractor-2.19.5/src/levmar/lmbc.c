/*
*				lmbc.c
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	AstrOmatic software
*
*	Copyright:		(C) 2007-2012 Emmanuel Bertin -- IAP/CNRS/UPMC
*				(C) 2004-2011 Manolis Lourakis (orig. version)
*
*	Licenses:		GNU General Public License
*
*	AstrOmatic software is free software: you can redistribute it and/or
*	modify it under the terms of the GNU General Public License as
*	published by the Free Software Foundation, either version 3 of the
*	License, or (at your option) any later version.
*	AstrOmatic software is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with AstrOmatic software.
*	If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		09/07/2012
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/////////////////////////////////////////////////////////////////////////////////
//
//  Levenberg - Marquardt non-linear minimization algorithm
//  Copyright (C) 2004-05  Manolis Lourakis (lourakis at ics forth gr)
//  Institute of Computer Science, Foundation for Research & Technology - Hellas
//  Heraklion, Crete, Greece.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
/////////////////////////////////////////////////////////////////////////////////

/******************************************************************************** 
 * Box-constrained Levenberg-Marquardt nonlinear minimization. The same core code
 * is used with appropriate #defines to derive single and double precision versions,
 * see also lmbc_core.c
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "levmar.h"
#include "compiler.h"
#include "misc.h"

#define EPSILON 1E-12
#define ONE_THIRD 0.3333333334 /* 1.0/3.0 */
#define _LSITMAX_ 150          /* max #iterations for line search */
#define _POW_ 2.1

#if !defined( LM_DBL_PREC ) && !defined( LM_SNGL_PREC )
#error At least one of LM_DBL_PREC, LM_SNGL_PREC should be defined!
#endif

#ifdef LM_SNGL_PREC
/* single precision (float) definitions */
#define LM_REAL float
#define LM_PREFIX s

#define LM_REAL_MAX FLT_MAX
#define LM_REAL_MIN -FLT_MAX

#define LM_REAL_EPSILON FLT_EPSILON
#define __SUBCNST( x ) x##F
#define LM_CNST( x ) __SUBCNST( x ) // force substitution

#include "lmbc_core.c" // read in core code

#undef LM_REAL
#undef LM_PREFIX
#undef LM_REAL_MAX
#undef LM_REAL_MIN
#undef LM_REAL_EPSILON
#undef __SUBCNST
#undef LM_CNST
#endif /* LM_SNGL_PREC */

#ifdef LM_DBL_PREC
/* double precision definitions */
#define LM_REAL double
#define LM_PREFIX d

#define LM_REAL_MAX DBL_MAX
#define LM_REAL_MIN -DBL_MAX

#define LM_REAL_EPSILON DBL_EPSILON
#define LM_CNST( x ) ( x )

#include "lmbc_core.c" // read in core code

#undef LM_REAL
#undef LM_PREFIX
#undef LM_REAL_MAX
#undef LM_REAL_MIN
#undef LM_REAL_EPSILON
#undef LM_CNST
#endif /* LM_DBL_PREC */
