/** 
 * @file rapi.c
 * @brief 
 * @author The vle Development Team
 * @date 2007-11-14
 */

/*
 * Copyright (C) 2007 - The vle Development Team
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

/*
 *
 * forward declarations
 *
 */

static SEXP r_rvle_open(SEXP name);
static void r_rvle_run(SEXP rvle);
static void r_rvle_close(SEXP rvle);
static void r_rvle_delete(SEXP rvle);

/*
 *
 * R function registration
 *
 */

R_CallMethodDef callMethods[] =
{
        { "open", r_rvle_open, 1},
        { "run", r_rvle_run, 1},
        { "close", r_rvle_close, 1},
        { NULL, NULL, 0}
};

void R_init_rvle(DllInfo* info)
{
#ifndef NDEBUG
        printf("RVLE: R_init_rvle\n");
#endif
        R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}

void R_unload_rvle(DllInfo* info)
{
#ifndef NDEBUG
        printf("RVLE: R_unload_rvle\n");
#endif
}

/*
 *
 * rvle
 *
 */

#include "rvle.h"

SEXP r_rvle_open(SEXP name)
{
        SEXP r = R_NilValue;

        void* p = (void*) rvle_open(CHAR(STRING_ELT(name, 0)));
        if (p) {
                PROTECT(r = R_MakeExternalPtr(p, R_NilValue, R_NilValue));
                R_RegisterCFinalizer(r, r_rvle_delete);
                UNPROTECT(1);
        }

        return r;
}

void r_rvle_run(SEXP rvle)
{
        rvle_run(R_ExternalPtrAddr(rvle));
}

void r_rvle_close(SEXP rvle)
{
        rvle_close(R_ExternalPtrAddr(rvle));
}

void r_rvle_delete(SEXP rvle)
{
        rvle_delete(R_ExternalPtrAddr(rvle));
}
