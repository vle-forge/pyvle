/**
 * @file        pyvle.cpp
 * @author      The VLE Development Team.
 * @brief       Python wrapper of VLE
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
#include <vle/value.hpp>
#include <vle/utils.hpp>
#include "convert.hpp"
#include "pyvle.hpp"

using namespace vle;

static bool thread_init = false;

vpz::Vpz* pyvle_open(const char* filename)
{
    vpz::Vpz* file = 0;

    try {
	if (!thread_init) {
	    vle::utils::init();
	    thread_init = true;
	}
        file = new vpz::Vpz(filename);
        return file;
    } catch(const std::exception& e) {
        return NULL;
    }
}

void pyvle_delete(vle::vpz::Vpz* file)
{
    assert(file);

    delete file;
}

PyObject* pyvle_run(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::RunQuiet jrm;

        jrm.start(*file);
        const oov::OutputMatrixViewList& result(jrm.outputs());

	return pyvle_convert_dataframe(result);
    } catch(const std::exception& e) {
	return NULL;
    }
}

PyObject* pyvle_run_matrix(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::RunQuiet jrm;

        jrm.start(*file);
        const oov::OutputMatrixViewList& result(jrm.outputs());

	return pyvle_convert_matrix(result);
    } catch(const std::exception& e) {
	return NULL;
    }
}

PyObject* pyvle_manager(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::ManagerRunMono jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_dataframe(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_manager_matrix(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::ManagerRunMono jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_matrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_manager_thread(vpz::Vpz* file, int th)
{
    assert(file);

    try {
        manager::ManagerRunThread jrm(std::cerr, false, th);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_dataframe(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_manager_thread_matrix(vpz::Vpz* file, int th)
{
    assert(file);

    try {
        manager::ManagerRunThread jrm(std::cerr, false, th);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_matrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_manager_cluster(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::ManagerRunDistant jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_dataframe(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_manager_cluster_matrix(vpz::Vpz* file)
{
    assert(file);

    try {
        manager::ManagerRunDistant jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
	return pyvle_convert_simulation_matrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_condition_list(vpz::Vpz* file)
{
    assert(file);
    
    PyObject* r;    /* condition list result */
    int size;       /* size of the condition list from the vle api */
    int i;

    size = file->project().experiment().conditions().conditionlist().size();
    r = PyList_New(size);
    if (size > 0) {
	std::list < std::string > lst;

	file->project().experiment().conditions().conditionnames(lst);

	std::list < std::string >::const_iterator it;
	
	i = 0;
	for (it = lst.begin(); it != lst.end(); ++it, ++i) 
	    PyList_SetItem(r, i, PyString_FromString(it->c_str()));
    }
    return r;
}

PyObject* pyvle_condition_show(vle::vpz::Vpz* file, 
			       std::string conditionname, 
			       std::string portname)
{
    assert(file);
    
    PyObject* r;    /* condition list result */
    
    vle::vpz::Condition& cnd(file->project().experiment().
			     conditions().get(conditionname));
    vle::value::VectorValue& v(cnd.getSetValues(portname).value());
    int size = v.size();

    if (size > 1) {
	r = PyList_New(size);
	for (int i = 0; i < size; ++i)
	  PyList_SetItem(r, i, pyvle_convert_value(*v[i]));  
    } else {
	r = pyvle_convert_value(*v[0]);
    }
    return r;
}

PyObject* pyvle_condition_size(vpz::Vpz* file)
{
    assert(file);
    
    return PyInt_FromLong(file->project().experiment().conditions().conditionlist().size());
}

PyObject* pyvle_condition_port_list_size(vpz::Vpz* file, std::string conditionname)
{
    assert(file);
    
    int result;

    try {
        vpz::Condition& cnd(
            file->project().experiment().conditions().get(conditionname));
        result = cnd.conditionvalues().size();
    } catch(const std::exception& e) {
        result = -1;
    }
    return PyInt_FromLong(result);
}

PyObject* pyvle_condition_port_list(vpz::Vpz* file, std::string conditionname)
{
    assert(file);
    
    PyObject* r;    /* port list result */
    int size;       /* size of the port list from the vle api */
    int i;
    const vpz::Condition& cnd(
	file->project().experiment().conditions().get(conditionname));

    size = cnd.conditionvalues().size();
    r = PyList_New(size);
    if (size > 0) {
	std::list < std::string > lst;

        cnd.portnames(lst);

	std::list < std::string >::const_iterator it;
	
	i = 0;
	for (it = lst.begin(); it != lst.end(); ++it, ++i) 
	    PyList_SetItem(r, i, PyString_FromString(it->c_str()));
    }
    return r;
}

void pyvle_condition_clear(vpz::Vpz* file, 
		     std::string conditionname, 
		     std::string portname)
{
    assert(file);
    
    vpz::Condition& cnd(file->project().experiment().
			conditions().get(conditionname));
    
    cnd.clearValueOfPort(portname);
}

void pyvle_condition_add_real(vpz::Vpz* file, 
			      std::string conditionname, 
			      std::string portname, 
			      double value)
{
    assert(file);
    
    vpz::Condition& cnd(file->project().experiment().
			conditions().get(conditionname));
    
    cnd.addValueToPort(portname, value::Double::create(value));
}

void pyvle_condition_add_integer(vpz::Vpz* file, 
				 std::string conditionname, 
				 std::string portname, 
				 long value)
{
    assert(file);
    
    vpz::Condition& cnd(file->project().experiment().
			conditions().get(conditionname));
    
    cnd.addValueToPort(portname, value::Integer::create(value));
}

void pyvle_condition_add_string(vpz::Vpz* file, 
				std::string conditionname, 
				std::string portname, 
				std::string value)
{
    assert(file);
    
    vpz::Condition& cnd(file->project().experiment().
			conditions().get(conditionname));
    
    cnd.addValueToPort(portname, value::String::create(value));
}

void pyvle_experiment_set_duration(vpz::Vpz* file, 
			     double value)
{
    assert(file);
    
    file->project().experiment().setDuration(value);
}

PyObject* pyvle_experiment_get_duration(vpz::Vpz* file)
{
    assert(file);
    
    return PyFloat_FromDouble(file->project().experiment().duration());
}

void pyvle_experiment_set_seed(vpz::Vpz* file, 
			       double value)
{
    assert(file);
    
    file->project().experiment().setSeed(value);
}

PyObject* pyvle_experiment_get_seed(vpz::Vpz* file)
{
    assert(file);
    
    return PyFloat_FromDouble(file->project().experiment().seed());
}

void pyvle_save(vpz::Vpz* file, 
	  std::string filename)
{
    assert(file);
    
    file->write(filename);
}
