/** 
 * @file convert.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vle/manager.hpp>
#include <vle/oov.hpp>
#include <vle/value.hpp>
#include <cassert>
#include "rvle.h"
#include "convert.h"

using namespace vle;


static void rvle_convert_view(const oov::OutputMatrix& matrix, SEXP out)
{
    value::MatrixFactory::ConstMatrixView view(matrix.values());
    value::MatrixFactory::ConstMatrixView::index i, j;

    for (i = 0; i < view.shape()[0]; ++i) {
        for (j = 0; j < view.shape()[1]; ++j) {
            switch (view[i][j]->getType()) {
            case value::ValueBase::BOOLEAN: {
                REAL(out)[j + i * view.shape()[1]] = (double)
                    value::toBoolean(view[i][j]);
                break;
            }
            case value::ValueBase::DOUBLE: {
                REAL(out)[j + i * view.shape()[1]] = 
                    value::toDouble(view[i][j]);
                break;
            }
            case value::ValueBase::INTEGER: {
                REAL(out)[j + i * view.shape()[1]] = (double)
                    value::toInteger(view[i][j]);
                break;
            }
            default: {
                REAL(out)[j + i * view.shape()[1]] = NA_REAL;
                break;
            }
            }
        }
    }
}

//
// public R part
//

SEXP rvle_convert_simulation_matrix(rvle_output_t out)
{
    SEXP r;

    manager::OutputSimulationMatrix* matrix(
        reinterpret_cast < manager::OutputSimulationMatrix* >(out));

    PROTECT(r = allocMatrix(VECSXP, matrix->shape()[0], matrix->shape()[1]));

    manager::OutputSimulationMatrix::index i, j;
    for (i = 0; i < matrix->shape()[0]; ++i) {
        for (j = 0; j < matrix->shape()[1]; ++j) {
            const oov::OutputMatrixViewList& lst((*matrix)[i][j]);

            SEXP sexplst;
            PROTECT(sexplst = allocVector(VECSXP, lst.size()));
            SET_VECTOR_ELT(r, i + j * matrix->shape()[1], sexplst);

            oov::OutputMatrixViewList::const_iterator it;
            int n;
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
                SEXP sexpdata;
                PROTECT(sexpdata = allocMatrix(REALSXP,
                                               it->second.values().shape()[1],
                                               it->second.values().shape()[0]));
                rvle_convert_view(it->second, sexpdata);

                SET_VECTOR_ELT(sexplst, n, sexpdata);
            }
            UNPROTECT(lst.size());
        }
        UNPROTECT(matrix->shape()[0] * matrix->shape()[1]);
    }
    UNPROTECT(1);
    return r;
}

