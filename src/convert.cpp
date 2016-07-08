/*
 * @file src/convert.cpp
 *
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems
 * http://www.vle-project.org
 *
 * Copyright (c) 2003-2007 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (c) 2003-2010 ULCO http://www.univ-littoral.fr
 * Copyright (c) 2007-2010 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and contributors
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


#include <convert.hpp>
#include <boost/format.hpp>
#include <cassert>
#include <vle/value/Tuple.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Matrix.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/Table.hpp>


PyObject* pyvle_convert_value(const vle::value::Value& value)
{
    PyObject *pyvle = PyImport_ImportModule("pyvle");
    PyObject* result;

    switch (value.getType()) {
    case vle::value::Value::BOOLEAN: {
        result = PyBool_FromLong(
                vle::value::toBoolean(value));
        break;
    }
    case vle::value::Value::INTEGER: {
        result = PyInt_FromLong(
                vle::value::toInteger(value));
        break;
    }
    case vle::value::Value::DOUBLE: {
        result = PyFloat_FromDouble(
                vle::value::toDouble(value));
        break;
    }
    case vle::value::Value::STRING: {
        result = PyString_FromString(
                vle::value::toString(value).c_str());
        break;
    }
    case vle::value::Value::XMLTYPE: {
        PyObject *class_ = PyObject_GetAttrString(pyvle, "VleXML");
        PyObject* val = PyString_FromString(
                vle::value::toXml(value).c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, val);
        result = PyInstance_New(class_, args, NULL);
        break;
    }
    case vle::value::Value::SET: {
        result = PyList_New(0);
        for (vle::value::Set::const_iterator it = value.toSet().begin();
            it != value.toSet().end(); ++it) {
            PyList_Append(result, pyvle_convert_value(**it));
        }
        break;
    }
    case vle::value::Value::MAP: {
        result = PyDict_New();
        for (vle::value::Map::const_iterator it = value.toMap().begin();
            it != value.toMap().end(); ++it) {
            PyDict_SetItemString(result, it->first.c_str(),
                                 pyvle_convert_value(*(it->second)));
        }
        break;
    }
    case vle::value::Value::TUPLE: {
        PyObject *class_ = PyObject_GetAttrString(pyvle, "VleTuple");
        PyObject* val = PyList_New(0);
        std::vector<double>::const_iterator itb =
                value.toTuple().value().begin();
        std::vector<double>::const_iterator ite =
                value.toTuple().value().end();
        for(;itb!=ite;itb++){
            PyList_Append(val, PyFloat_FromDouble(*itb));
        }
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, val);
        result = PyInstance_New(class_, args, NULL);
        break;
    }
    case vle::value::Value::TABLE: {
        PyObject *class_ = PyObject_GetAttrString(pyvle, "VleTable");
        PyObject* val = PyList_New(0);
        PyObject* r=0;
        const vle::value::Table& t = value.toTable();
        for(unsigned int i=0; i<t.height(); i++){
            r = PyList_New(0);
            for(unsigned int j=0; j<t.width(); j++){
                PyList_Append(r,PyFloat_FromDouble(t.get(j,i)));
            }
            PyList_Append(val,r);
        }
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, val);
        result = PyInstance_New(class_, args, NULL);
        break;
    }
    case vle::value::Value::MATRIX: {
        PyObject *class_ = PyObject_GetAttrString(pyvle, "VleMatrix");
        PyObject* val = PyList_New(0);
        PyObject* r=0;
        const vle::value::Matrix& t = value.toMatrix();
        for(unsigned int i=0; i<t.rows(); i++){
            r = PyList_New(0);
            for(unsigned int j=0; j<t.columns(); j++){
                PyList_Append(r,pyvle_convert_value(*t.get(j,i)));
            }
            PyList_Append(val,r);
        }
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, val);
        result = PyInstance_New(class_, args, NULL);
        break;
    }
    default: {
        result = Py_None;
        break;
    }
    }
    return result;
}
/**
 * @brief Converts a view : lines represent time, columns represent
 * obserrvation ports
 *
 * @param matrix, A matrix that is a view.
 *
 * @return A Python representation of: a Matrix.
 */
PyObject* pyvle_convert_view_matrix(const vle::value::Matrix& matrix)
{
    vle::value::Matrix::size_type i, j;
    vle::value::Matrix::size_type nbcols = matrix.columns();
    vle::value::Matrix::size_type nbrows = matrix.rows();

    PyObject* out = PyTuple_New(nbcols);

    for (i = 0; i < nbcols; ++i) {
        PyObject* column = PyTuple_New(nbrows);
        for (j = 0; j < nbrows; ++j) {
            const std::unique_ptr<vle::value::Value>& v = matrix.get(i, j);
            if (v) {
                PyTuple_SetItem(column, j,
                        pyvle_convert_value(*v));
            } else {
                PyTuple_SetItem(column, j, Py_None);
            }
        }
        PyTuple_SetItem(out, i, column);
    }
    return out;
}

/**
 * @brief Converts a view : lines represent time, columns represent
 * observation ports
 *
 * @param matrix, A matrix that is a view.
 *
 * @return A Python representation of: a Map.
 */
PyObject* pyvle_build_data_frame(const vle::value::Matrix& matrix)
{
    //assumption, first line contains name of the columns
    PyObject* out = PyDict_New();

    unsigned int nbcol = matrix.columns();
    unsigned int nbline = matrix.rows();

    for(unsigned int c = 0; c < nbcol; c++){
        PyObject* col = PyTuple_New(nbline - 1);
        for (unsigned int i = 1; i < nbline; ++i) {
            const std::unique_ptr<vle::value::Value>& v = matrix.get(c,i);
            if (v) {
                PyTuple_SetItem(col, i-1, pyvle_convert_value(*v));
            } else {
                PyTuple_SetItem(col, i-1, Py_None);
            }
        }
        PyDict_SetItemString(out, matrix.getString(c,0).c_str(), col);
    }
    return out;
}

PyObject* pyvle_convert_matrix(const vle::value::Map& out)
{
    PyObject* plst;
    plst = PyDict_New();
    vle::value::Map::const_iterator itb = out.begin();
    vle::value::Map::const_iterator ite = out.end();
    for(;itb!=ite;itb++){
        PyObject* pdata = pyvle_convert_view_matrix(itb->second->toMatrix());
        PyDict_SetItemString(plst, itb->first.c_str(), pdata);
    }
    return plst;
}

PyObject* pyvle_convert_dataframe(const vle::value::Map& out)
{

    PyObject* plst;
    plst = PyDict_New();
    vle::value::Map::const_iterator itb = out.begin();
    vle::value::Map::const_iterator ite = out.end();
    for(;itb!=ite;itb++){
        PyObject* pdata = pyvle_build_data_frame(itb->second->toMatrix());
        PyDict_SetItemString(plst, itb->first.c_str(), pdata);
    }
    return plst;
}

PyObject* pyvle_convert_simulation_matrix(const vle::value::Matrix& out)
{
    PyObject* line;
    PyObject* columns = PyTuple_New(out.columns());
    for (unsigned int j=0; j<out.columns();j++){
        line = PyTuple_New(out.rows());
        for (unsigned int i=0; i<out.rows();i++){
            PyObject* pdata = pyvle_convert_matrix(out.get(j,i)->toMap());
            PyTuple_SetItem(line, i, pdata);
        }
        PyTuple_SetItem(columns, j, line);
    }
    return columns;
}



PyObject* pyvle_convert_simulation_dataframe(const vle::value::Matrix& out)
{
    PyObject* line;
    PyObject* columns = PyTuple_New(out.columns());
    for (unsigned int j=0; j<out.columns();j++){
        line = PyTuple_New(out.rows());
        for (unsigned int i=0; i<out.rows();i++){
            PyObject* pdata = pyvle_convert_dataframe(out.get(j,i)->toMap());
            PyTuple_SetItem(line, i, pdata);
        }
        PyTuple_SetItem(columns, j, line);
    }
    return columns;
}
