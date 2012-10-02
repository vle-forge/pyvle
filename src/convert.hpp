/*
 * @file src/convert.hpp
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


#ifndef VLE_PORT_PYVLE_CONVERT_H
#define VLE_PORT_PYVLE_CONVERT_H 1

#include <Python.h>
#include <vle/manager/Manager.hpp>
#include <vle/value/Matrix.hpp>
#include <vle/value/Map.hpp>
#include <vle/value/Boolean.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/String.hpp>
#include <vle/value/XML.hpp>

/**
 * @brief Converts a simple vle::Value
 *
 * @param value, a vle Value.
 *
 * @return A Python representation of: a Matrix.
 */
PyObject* pyvle_convert_value(const vle::value::Value& value);

/**
 * @brief Build an output of simulations result into a Map (each cell is a
 * list of view) of Matrix of real (the result of the simulation). Values of
 * type boolean, integer or double from the vle are transformed into double
 * value, other value return None.
 *
 * @param out The output of simulations.
 *
 * @return A Python representation of: List of Matrix.
 */
PyObject* pyvle_convert_matrix(const vle::value::Map& out);

/**
 * @brief Build an output of simulations result into a Tuple (each cell is a
 * list of view) of Map. Values of type boolean, integer, double, string
 * conserve their type but by column. If in a column, different values are
 * returned, the value are set to None. Only the first value of the
 * column is used to get the type of the column.
 *
 * @param out The output of simulations.
 *
 * @return A Python representation of: List of Maps.
 */
PyObject* pyvle_convert_dataframe(const vle::value::Map& out);

/**
 * @brief Build an output of simulations result into a Matrix (replicas x
 * combination) of Tuple (each cell is a list of view) of Matrix of real (the
 * result of the simulation). Values of type boolean, integer or double from the
 * vle are transformed into double value, other value return None.
 *
 * @param out The output of simulations.
 *
 * @return A Python representation of: Matrix of List of Matrix.
*/
PyObject* pyvle_convert_simulation_matrix(const vle::value::Matrix& out);

/**
 * @brief Build an output of simulations result into a Matrix (replicas x
 * conbination) of Tuple (each cell is a list of view) of Map. Values of
 * type boolean, integer, double, string conserve their type but by column. If
 * in a column, different values are returned, the value are set to
 * None. Only the first value of the column is used to get the type of
 * the column.
 *
 * @param out The output of simulations.
 *
 * @return A Python representation of: Matrix of List of Maps.
 */
PyObject* pyvle_convert_simulation_dataframe(const vle::value::Matrix& out);



#endif
