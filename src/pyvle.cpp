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
	    vle::manager::init();
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

PyObject* pyvle_dynamics_list(vpz::Vpz* file)
{
	assert(file);

	PyObject* r;    /* dynamics list result */
	int size;       /* size of the dynamics list from the vle api */
	int i;
	vpz::DynamicList& dynlist(file->project().dynamics().dynamiclist());
	vpz::DynamicList::iterator dynit;

	size = dynlist.size();

	r = PyList_New(size);
	i = 0;
	if (size > 0) {
		for (dynit = dynlist.begin(); dynit != dynlist.end(); ++dynit, ++i) {
			PyList_SetItem(r, i, PyString_FromString(dynit->first.c_str()));
		}
	}
	return r;
}

PyObject* pyvle_dynamic_get_name(vle::vpz::Vpz* file,
				std::string dynamicname)
{
	assert(file);

	PyObject* r;    /* dynamic name result */
	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));

	r = PyString_FromString(dyn.name().c_str());

	return r;
}

PyObject* pyvle_dynamic_get_model(vle::vpz::Vpz* file,
				std::string dynamicname)
{
	assert(file);

	PyObject* r;    /* dynamic model result */
	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));

	r = PyString_FromString(dyn.model().c_str());

	return r;
}

PyObject* pyvle_dynamic_get_library(vle::vpz::Vpz* file,
				std::string dynamicname)
{
	assert(file);

	PyObject* r;    /* dynamic library result */
	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));

	r = PyString_FromString(dyn.library().c_str());

	return r;
}

PyObject* pyvle_dynamic_get_language(vle::vpz::Vpz* file,
				std::string dynamicname)
{
	assert(file);

	PyObject* r;    /* dynamic language result */
	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));

	r = PyString_FromString(dyn.language().c_str());

	return r;
}

void pyvle_dynamic_set_model(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string model)
{
	assert(file);

	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));
	dyn.setModel(model);
}

void pyvle_dynamic_set_library(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string library)
{
	assert(file);

	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));
	dyn.setLibrary(library);
}

void pyvle_dynamic_set_language(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string language)
{
	assert(file);

	vpz::Dynamic& dyn(file->project().dynamics().get(dynamicname));
	dyn.setLanguage(language);
}

PyObject* pyvle_views_list(vle::vpz::Vpz* file)
{
	assert(file);

	PyObject* r;    /* views list result */
	vpz::ViewList& viewslst(file->project().experiment().views().viewlist());
	vpz::ViewList::iterator it;
	int size;
	int i;

	size = viewslst.size();

	r = PyList_New(size);
	i = 0;

	if (size > 0) {
		for (it = viewslst.begin(); it != viewslst.end(); ++it, ++i) {
			PyList_SetItem(r, i, PyString_FromString(it->first.c_str()));
		}
	}
	return r;
}

PyObject* pyvle_view_get_name(vle::vpz::Vpz* file,
				std::string viewname)
{
	assert(file);

	PyObject* r;    /* view name result */
	vpz::View& view(file->project().experiment().views().get(viewname));

	r = PyString_FromString(view.name().c_str());

	return r;
}

PyObject* pyvle_view_get_type(vle::vpz::Vpz* file,
				std::string viewname)
{
	assert(file);

	PyObject* r;    /* view type result */
	vpz::View& view(file->project().experiment().views().get(viewname));

	r = PyString_FromString(view.streamtype().c_str());

	return r;
}

PyObject* pyvle_view_get_timestep(vle::vpz::Vpz* file,
				std::string viewname)
{
	assert(file);

	PyObject* r;    /* view timestep result */
	vpz::View& view(file->project().experiment().views().get(viewname));

	r = PyFloat_FromDouble(view.timestep());

	return r;
}

PyObject* pyvle_view_get_output(vle::vpz::Vpz* file,
				std::string viewname)
{
	assert(file);

	PyObject* r;    /* view output result */
	vpz::View& view(file->project().experiment().views().get(viewname));

	r = PyString_FromString(view.output().c_str());

	return r;
}

PyObject* pyvle_view_get_data(vle::vpz::Vpz* file,
		std::string viewname)
{
	assert(file);

	PyObject* r;    /* view data result */
	vpz::View& view(file->project().experiment().views().get(viewname));

	r = PyString_FromString(view.data().c_str());

	return r;
}

void pyvle_view_set_name(vle::vpz::Vpz* file,
				std::string viewoldname,
				std::string viewnewname)
{
	assert(file);

	vpz::View& view(file->project().experiment().views().get(viewoldname));

	view.setName(viewnewname);
}

void pyvle_view_set_type(vle::vpz::Vpz* file,
				std::string viewname,
				std::string viewtype)
{
	assert(file);

	vpz::View& view(file->project().experiment().views().get(viewname));

	if (viewtype=="TIMED")
		view.setType(vpz::View::TIMED);
	else if (viewtype=="EVENT")
		view.setType(vpz::View::EVENT);
	else if (viewtype=="FINISH")
		view.setType(vpz::View::FINISH);
}

void pyvle_view_set_timestep(vle::vpz::Vpz* file,
				std::string viewname,
				double time)
{
	assert(file);

	vpz::View& view(file->project().experiment().views().get(viewname));

	if (time >= 0.0)
		view.setTimestep(time);
}

void pyvle_view_set_data(vle::vpz::Vpz* file,
				std::string viewname,
				std::string data)
{
	assert(file);

	vpz::View& view(file->project().experiment().views().get(viewname));

	view.setData(data);
}

void pyvle_views_add_eventview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output)
{
	assert(file);

	file->project().experiment().views().addEventView(viewname,output);
}

void pyvle_views_add_timedview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output,
				double time)
{
	assert(file);

	file->project().experiment().views().addTimedView(viewname,time,output);
}

void pyvle_views_add_finishview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output)
{
	assert(file);

	file->project().experiment().views().addFinishView(viewname,output);
}

PyObject* pyvle_observables_list(vle::vpz::Vpz* file)
{
	assert(file);

	PyObject* r;    /* observables list result */
	vpz::ObservableList& obslst(file->project().experiment().views().observables().observablelist());
	vpz::ObservableList::iterator it;
	int size;
	int i;

	size = obslst.size();

	r = PyList_New(size);
	i = 0;

	if (size > 0) {
		for (it = obslst.begin(); it != obslst.end(); ++it, ++i) {
			PyList_SetItem(r, i, PyString_FromString(it->first.c_str()));
		}
	}
	return r;
}

void pyvle_observable_add(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	vpz::Observable newobs(obsname);

	file->project().experiment().views().observables().add(newobs);
}

void pyvle_observable_del(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	file->project().experiment().views().observables().del(obsname);
}

PyObject* pyvle_observable_exists(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	if (file->project().experiment().views().observables().exist(obsname)) {
		return Py_True;
	} else {
		return Py_False;
	}
}

void pyvle_observables_clear(vle::vpz::Vpz* file)
{
	assert(file);

	file->project().experiment().views().observables().clear();
}

PyObject* pyvle_observables_empty(vle::vpz::Vpz* file)
{
	assert(file);

	if (file->project().experiment().views().observables().empty()) {
		return Py_True;
	} else {
		return Py_False;
	}
}

PyObject* pyvle_observable_get_name(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	PyObject* r;    /* observable name list result */
	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	r = PyString_FromString(obs.name().c_str());

	return r;
}

PyObject* pyvle_observable_ports_list(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	PyObject* r;    /* observable port list result */
	vpz::ObservablePortList& obsportlst(file->project().experiment().views().observables().get(obsname).observableportlist());
	vpz::ObservablePortList::iterator it;
	int size;
	int i;

	size = obsportlst.size();

	r = PyList_New(size);
	i = 0;

	if (size > 0) {
		for (it = obsportlst.begin(); it != obsportlst.end(); ++it, ++i) {
			PyList_SetItem(r, i, PyString_FromString(it->first.c_str()));
		}
	}
	return r;
}

void pyvle_observable_add_port(vle::vpz::Vpz* file,
				std::string obsname,
				std::string portname)
{
	assert(file);

	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	if (!obs.exist(portname)) {
		obs.add(portname);
	}

}

void pyvle_observable_del_port(vle::vpz::Vpz* file,
				std::string obsname,
				std::string portname)
{
	assert(file);

	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	if (obs.exist(portname)) {
		obs.del(portname);
	}

}

PyObject* pyvle_observable_has_view(vle::vpz::Vpz* file,
				std::string obsname,
				std::string viewname)
{
	assert(file);

	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	if (obs.hasView(viewname)) {
		return Py_True;
	} else {
		return Py_False;
	}
}

PyObject* pyvle_observable_get_port_name(vle::vpz::Vpz* file,
				std::string obsname,
				std::string viewname)
{
	assert(file);

	PyObject* r;    /* port list result */
	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));
	vpz::PortNameList obsportlist = obs.getPortname(viewname);
	vpz::PortNameList::iterator it;
	int size;
	int i;

	size = obsportlist.size();

	r = PyList_New(size);
	i = 0;

	if (size > 0) {
		for (it = obsportlist.begin(); it != obsportlist.end(); ++it, ++i) {
			PyList_SetItem(r, i, PyString_FromString(it->c_str()));
		}
	}
	return r;
}

PyObject* pyvle_observable_is_permanent(vle::vpz::Vpz* file,
				std::string obsname)
{
	assert(file);

	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	if (obs.isPermanent()) {
		return Py_True;
	} else {
		return Py_False;
	}
}

void pyvle_observable_set_permanent(vle::vpz::Vpz* file,
				std::string obsname,
				bool ispermanent)
{
	assert(file);

	vpz::Observable& obs(file->project().experiment().views().observables().get(obsname));

	obs.permanent(ispermanent);
}
