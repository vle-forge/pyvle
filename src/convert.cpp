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
#include <vle/value/Boolean.hpp>
#include <cassert>
#include <Rdefines.h>
#include "rvle.h"
#include "convert.h"

using namespace vle;

static void rvle_convert_view_matrix(const oov::OutputMatrix& matrix, SEXP out)
{
    value::ConstMatrixView view(matrix.values());
    value::ConstMatrixView::index i, j;

    for (i = 0; i < view.shape()[0]; ++i) {
        for (j = 0; j < view.shape()[1]; ++j) {
            if (view[i][j]) {
                switch (view[i][j]->getType()) {
                case value::Value::BOOLEAN:
                    REAL(out)[j + i * view.shape()[1]] = (double)
                        value::toBoolean(view[i][j]);
                    break;
                case value::Value::DOUBLE:
                    REAL(out)[j + i * view.shape()[1]] =
                        value::toDouble(view[i][j]);
                    break;
                case value::Value::INTEGER:
                    REAL(out)[j + i * view.shape()[1]] = (double)
                        value::toInteger(view[i][j]);
                    break;
                default:
                    REAL(out)[j + i * view.shape()[1]] = NA_REAL;
                    break;
                }
            } else {
                REAL(out)[j + i * view.shape()[1]] = NA_REAL;
            }
        }
    }
}

static void rvle_convert_vector_boolean(
    const value::ConstVectorView& vec,
    SEXP out)
{
    int i = 0;
    for (value::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if (*it == 0 || ((*it)->getType() != value::Value::BOOLEAN)) {
            LOGICAL(out)[i] = NA_LOGICAL;
        } else {
            LOGICAL(out)[i] = value::toBoolean(*it);
        }
        ++i;
    }
}

static void rvle_convert_vector_double(
    const value::ConstVectorView& vec,
    SEXP out)
{
    int i = 0;
    for (value::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if (*it == 0 || ((*it)->getType() != value::Value::DOUBLE)) {
            REAL(out)[i] = NA_REAL;
        } else {
            REAL(out)[i] = value::toDouble(*it);
        }
        ++i;
    }
}

static void rvle_convert_vector_integer(
    const value::ConstVectorView& vec,
    SEXP out)
{
    int i = 0;
    for (value::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if (*it == 0 || (*it)->getType() != value::Value::INTEGER) {
            INTEGER(out)[i] = NA_INTEGER;
        } else {
            INTEGER(out)[i] = value::toInteger(*it);
        }
        ++i;
    }
}

static void rvle_convert_vector_string(
    const value::ConstVectorView& vec,
    SEXP out)
{
    int i = 0;
    for (value::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if (*it == 0 || (*it)->getType() != value::Value::STRING) {
            SET_STRING_ELT(out, i, NA_STRING);
        } else {
            SET_STRING_ELT(out, i, mkChar(value::toString(*it).c_str()));
        }
        ++i;
    }
}

static SEXP rvle_build_data_frame(const oov::OutputMatrix& matrix)
{
    value::ConstMatrixView view(matrix.values());
    SEXP ret, names, value;

    PROTECT(ret = NEW_LIST(view.shape()[0]));
    PROTECT(names = NEW_CHARACTER(view.shape()[0]));

    PROTECT(value = NEW_NUMERIC(view.shape()[1]));
    rvle_convert_vector_double(matrix.getValue(0), value);
    SET_VECTOR_ELT(ret, 0, value);
    SET_STRING_ELT(names, 0, mkChar("time"));

    const oov::OutputMatrix::MapPairIndex& index(matrix.index());
    for (oov::OutputMatrix::MapPairIndex::const_iterator it = index.begin();
         it != index.end(); ++it) {

        SET_STRING_ELT(names, it->second,
                       mkChar(boost::str(boost::format("%1%.%2%") %
                                         it->first.first %
                                         it->first.second).c_str()));

        if (view[it->second][0] == 0) {
            UNPROTECT(2);
            error("empty value in (%d,0)\n", it->second);
        } else {
            switch(view[it->second][0]->getType()) {
            case value::Value::BOOLEAN:
                PROTECT(value = NEW_LOGICAL(view.shape()[1]));
                rvle_convert_vector_boolean(matrix.getValue(it->second),
                                            value);
                break;
            case value::Value::DOUBLE:
                PROTECT(value = NEW_NUMERIC(view.shape()[1]));
                rvle_convert_vector_double(matrix.getValue(it->second),
                                           value);
                break;
            case value::Value::INTEGER:
                PROTECT(value = NEW_INTEGER(view.shape()[1]));
                rvle_convert_vector_integer(matrix.getValue(it->second),
                                            value);
                break;
            case value::Value::STRING:
                PROTECT(value = NEW_CHARACTER(view.shape()[1]));
                rvle_convert_vector_string(matrix.getValue(it->second),
                                           value);
                break;
            default:
                UNPROTECT(2); // unprotect ret and names
                error("not suppored type for (%s, %s), column (%d)",
                      it->first.first.c_str(), it->first.second.c_str(),
                      it->second);
            }
            SET_VECTOR_ELT(ret, it->second, value);
        }
    }

    /* set the first column name's */
    PROTECT(value = NEW_CHARACTER(view.shape()[1]));
    for (int i = 0; i < view.shape()[1]; ++i) {
        SET_STRING_ELT(value, i, mkChar(boost::str(boost::format(
                        "%1%") % i ).c_str()));
    }
    setAttrib(ret, R_RowNamesSymbol, value);

    SET_NAMES(ret, names);
    SET_CLASS(ret, mkString("data.frame"));
    UNPROTECT(matrix.index().size() + 3);
    return ret;
}

//
// public R part
//

SEXP rvle_convert_matrix(rvle_output_t out)
{
    SEXP sexplst;

    oov::OutputMatrixViewList* lst(
        reinterpret_cast < oov::OutputMatrixViewList* >(out));

    PROTECT(sexplst = allocVector(VECSXP, lst->size()));

    oov::OutputMatrixViewList::const_iterator it;
    int n;
    for (it = lst->begin(), n = 0; it != lst->end(); ++it, ++n) {
        SEXP sexpdata;
        PROTECT(sexpdata = allocMatrix(REALSXP,
                                       it->second.values().shape()[1],
                                       it->second.values().shape()[0]));
        rvle_convert_view_matrix(it->second, sexpdata);

        SET_VECTOR_ELT(sexplst, n, sexpdata);
    }
    UNPROTECT(lst->size() + 1);

    return sexplst;
}

SEXP rvle_convert_vectorvalue(rvle_output_t out)
{
    SEXP sexplst;

    value::VectorValue* lst(reinterpret_cast < value::VectorValue* >(out));

    PROTECT(sexplst = NEW_NUMERIC(lst->size()));
    value::VectorValue::const_iterator it;
    int n;
    for (it = lst->begin(), n = 0; it != lst->end(); ++it, ++n) {
        if (*it) {
            switch ((*it)->getType()) {
            case value::Value::BOOLEAN:
                REAL(sexplst)[n] = (double) value::toBoolean(*it);
                break;
            case value::Value::DOUBLE:
                REAL(sexplst)[n] = value::toDouble(*it);
                break;
            case value::Value::INTEGER:
                REAL(sexplst)[n] = value::toInteger(*it);
                break;
            default:
                REAL(sexplst)[n] = NA_REAL;
                break;
            }
        } else {
            REAL(sexplst)[n] = NA_REAL;
        }
    }
    UNPROTECT(1);
    return sexplst;
}

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
            SET_VECTOR_ELT(r, i * matrix->shape()[1] + j, sexplst);

            oov::OutputMatrixViewList::const_iterator it;
            int n;
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
                SEXP sexpdata;
                PROTECT(sexpdata = allocMatrix(REALSXP,
                                               it->second.values().shape()[1],
                                               it->second.values().shape()[0]));
                rvle_convert_view_matrix(it->second, sexpdata);

                SET_VECTOR_ELT(sexplst, n, sexpdata);
            }
            UNPROTECT(lst.size());
        }
    }
    UNPROTECT(matrix->shape()[0] * matrix->shape()[1]);
    UNPROTECT(1);
    return r;
}

SEXP rvle_convert_dataframe(rvle_output_t out)
{
    SEXP sexplst;

    oov::OutputMatrixViewList* lst(
        reinterpret_cast < oov::OutputMatrixViewList* >(out));

    PROTECT(sexplst = allocVector(VECSXP, lst->size()));
    oov::OutputMatrixViewList::const_iterator it;
    int n;
    for (it = lst->begin(), n = 0; it != lst->end(); ++it, ++n) {
        SEXP sexpdata = rvle_build_data_frame(it->second);
        SET_VECTOR_ELT(sexplst, n, sexpdata);
    }
    UNPROTECT(lst->size() + 1);

    return sexplst;
}

SEXP rvle_convert_simulation_dataframe(rvle_output_t out)
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
            SET_VECTOR_ELT(r, i * matrix->shape()[1] + j, sexplst);

            oov::OutputMatrixViewList::const_iterator it;
            int n;
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
                SEXP sexpdata = rvle_build_data_frame(it->second);
                SET_VECTOR_ELT(sexplst, n, sexpdata);
            }
            UNPROTECT(lst.size());
        }
    }
    UNPROTECT(matrix->shape()[0] * matrix->shape()[1]);
    UNPROTECT(1);
    return r;
}

