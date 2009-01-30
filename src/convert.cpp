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

#include <boost/format.hpp>
#include <vle/value.hpp>
#include <cassert>
#include "convert.hpp"

PyObject* pyvle_convert_value(const vle::value::Value& value)
{
    PyObject* result;

    switch (value.getType()) {
    case vle::value::Value::BOOLEAN: {
	result = PyBool_FromLong(
	    vle::value::toBoolean(value));
	break;
    }
    case vle::value::Value::DOUBLE: {
	result = PyFloat_FromDouble(
	    vle::value::toDouble(value));
	break;
    }
    case vle::value::Value::INTEGER: {
	result = PyInt_FromLong(
	    vle::value::toLong(value));
	break;
    }
    case vle::value::Value::STRING: {
	result = PyString_FromString(
	    vle::value::toString(value).c_str());
	break;
    }
    default: {
	result = Py_None;
	break;
    }
    }
    return result;
}


PyObject* pyvle_convert_view_matrix(const vle::oov::OutputMatrix& matrix)
{
    vle::value::ConstMatrixView view(matrix.values());
    vle::value::ConstMatrixView::index i, j;

    PyObject* out = PyTuple_New(view.shape()[0]);

    for (i = 0; i < view.shape()[0]; ++i) {
	PyObject* column = PyTuple_New(view.shape()[1]);

        for (j = 0; j < view.shape()[1]; ++j)
	    PyTuple_SetItem(column, j, pyvle_convert_value(*view[i][j]));
	PyTuple_SetItem(out, i, column);
    }
    return out;
}

PyObject* pyvle_build_data_frame(const vle::oov::OutputMatrix& matrix)
{
    vle::value::ConstMatrixView view(matrix.values());
    vle::value::ConstMatrixView::index i, j;

    PyObject* out = PyDict_New();
    PyObject* time = PyTuple_New(view.shape()[1]);
    vle::value::ConstVectorView t = matrix.getValue(0);

    for (i = 0; i < view.shape()[1]; ++i)
	PyTuple_SetItem(time, i, pyvle_convert_value(*t[i]));
    PyDict_SetItemString(out, "time", time);

    const vle::oov::OutputMatrix::MapPairIndex& index(matrix.index());
    for (vle::oov::OutputMatrix::MapPairIndex::const_iterator it =
	     index.begin();
         it != index.end(); ++it) {

	PyObject* column = PyTuple_New(view.shape()[1]);

	i = it->second;
        for (j = 0; j < view.shape()[1]; ++j)
	    PyTuple_SetItem(column, j, pyvle_convert_value(*view[i][j]));
	PyDict_SetItemString(out, boost::str(boost::format("%1%.%2%") %
					     it->first.first %
					     it->first.second).c_str(), column);
    }
    return out;
}

//
// public part
//

PyObject* pyvle_convert_matrix(const vle::oov::OutputMatrixViewList& out)
{
    PyObject* plst;
    vle::oov::OutputMatrixViewList::const_iterator it;

    plst = PyList_New(out.size());

    int n;
    for (it = out.begin(), n = 0; it != out.end(); ++it, ++n) {
	PyObject* pdata = pyvle_convert_view_matrix(it->second);

	PyList_SetItem(plst, n, pdata);
    }
    return plst;
}

PyObject* pyvle_convert_simulation_matrix(const vle::manager::OutputSimulationMatrix& out)
{
    PyObject* r;
    vle::manager::OutputSimulationMatrix::index i, j;

    r = PyTuple_New(out.shape()[0]);
    for (i = 0; i < out.shape()[0]; ++i) {
	PyObject* column = PyTuple_New(out.shape()[1]);

        for (j = 0; j < out.shape()[1]; ++j) {
            const vle::oov::OutputMatrixViewList& lst(out[i][j]);
	    vle::oov::OutputMatrixViewList::const_iterator it;
	    PyObject* plst;
            int n;

	    plst = PyList_New(lst.size());
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
		PyObject* pdata = pyvle_convert_view_matrix(it->second);

		PyList_SetItem(plst, n, pdata);
            }
	    PyTuple_SetItem(column, j, plst);
        }
	PyTuple_SetItem(r, i, column);
    }
    return r;
}

PyObject* pyvle_convert_dataframe(const vle::oov::OutputMatrixViewList& out)
{
    PyObject* plst;
    vle::oov::OutputMatrixViewList::const_iterator it;

    plst = PyList_New(out.size());

    int n;
    for (it = out.begin(), n = 0; it != out.end(); ++it, ++n) {
	PyObject* pdata = pyvle_build_data_frame(it->second);

	PyList_SetItem(plst, n, pdata);
    }
    return plst;
}

PyObject* pyvle_convert_simulation_dataframe(const vle::manager::OutputSimulationMatrix& out)
{
    PyObject* r;
    vle::manager::OutputSimulationMatrix::index i, j;

    r = PyTuple_New(out.shape()[0]);
    for (i = 0; i < out.shape()[0]; ++i) {
	PyObject* column = PyTuple_New(out.shape()[1]);

        for (j = 0; j < out.shape()[1]; ++j) {
            const vle::oov::OutputMatrixViewList& lst(out[i][j]);
	    vle::oov::OutputMatrixViewList::const_iterator it;
	    PyObject* plst;
            int n;

	    plst = PyList_New(lst.size());
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
		PyObject* pdata = pyvle_build_data_frame(it->second);

		PyList_SetItem(plst, n, pdata);
            }
	    PyTuple_SetItem(column, j, plst);
        }
	PyTuple_SetItem(r, i, column);
    }
    return r;
}
