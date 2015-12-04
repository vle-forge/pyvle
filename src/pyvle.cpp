/*
 * @file src/pyvle.cpp
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


#include <sstream>
#include <fstream>
#include <convert.hpp>
#include <pyvle.hpp>
#include <vle/vle.hpp>
#include <vle/devs/RootCoordinator.hpp>
#include <vle/vpz/AtomicModel.hpp>
#include <vle/manager/Manager.hpp>
#include <vle/manager/Simulation.hpp>
#include <vle/value/Value.hpp>
#include <vle/utils/Package.hpp>
#include <vle/utils/Path.hpp>
#include <vle/utils/Trace.hpp>
#include <vle/utils/Tools.hpp>
#include <vle/utils/ModuleManager.hpp>
#include <glibmm/exception.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

using namespace vle;

static bool thread_init = false;

vpz::Vpz* pyvle_open_pkg(const char* pkgname, const char* filename)
{
    vpz::Vpz* file = 0;

    try {
        if (!thread_init) {
            vle::Init app;//TODO
            thread_init = true;
        }
        vle::utils::Package pack(pkgname);
        std::string filepath = pack.getExpFile(filename);
        file = new vpz::Vpz(filepath);
        return file;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

vpz::Vpz* pyvle_from_buffer(const std::string& buffer)
{
    vpz::Vpz* file = 0;

    try {
        if (!thread_init) {
            vle::Init app;//TODO
            thread_init = true;
        }
        file = new vpz::Vpz();
        file->parseMemory(buffer);
        return file;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

vpz::Vpz* pyvle_from_buffer_pkg(const char* pkgname, const std::string& buffer)
{
    vpz::Vpz* file = 0;

    try {
        if (!thread_init) {
            vle::Init app;//TODO
            thread_init = true;
        }
        file = new vpz::Vpz();
        file->parseMemory(buffer);
        return file;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

void pyvle_save(vpz::Vpz* file,
        std::string filename)
{
    assert(file);

    file->write(filename);
}

PyObject* pyvle_save_buffer(vpz::Vpz* file)
{
    assert(file);
    return PyString_FromString(file->writeToString().c_str());
}

void pyvle_delete(vle::vpz::Vpz* file)
{
    assert(file);

    delete file;
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

void pyvle_experiment_set_name(vpz::Vpz* file,
        const std::string& name)
{
    assert(file);
    file->project().experiment().setName(name);
}

void pyvle_experiment_set_begin(vpz::Vpz* file,
        double value)
{
    assert(file);

    file->project().experiment().setBegin(value);
}

PyObject* pyvle_experiment_get_begin(vpz::Vpz* file)
{
    assert(file);

    return PyFloat_FromDouble(file->project().experiment().begin());
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

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

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

void pyvle_experiment_set_seed(vpz::Vpz* file,
        double /*value*/)
{
    assert(file);

    //    file->project().experiment().setSeed(value); //TODO
}

PyObject* pyvle_experiment_get_seed(vpz::Vpz* file)
{
    assert(file);

    //    return PyFloat_FromDouble(file->project().experiment().seed()); //TODO
    return PyFloat_FromDouble(1.0);
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

void pyvle_experiment_set_linear_combination(vpz::Vpz* file,
        int /*seed*/, int /*replicas*/)
{
    assert(file);

    //TODO
    //    file->project().experiment().replicas().setSeed(seed);
    //    file->project().experiment().replicas().setNumber(replicas);
    //    file->project().experiment().setCombination("linear");
}

void pyvle_experiment_set_total_combination(vpz::Vpz* file,
        int /*seed*/, int /*replicas*/)
{
    assert(file);

    //TODO
    //    file->project().experiment().replicas().setSeed(seed);
    //    file->project().experiment().replicas().setNumber(replicas);
    //    file->project().experiment().setCombination("total");
}


/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */
PyObject* pyvle_run(vpz::Vpz* file)
{
    assert(file);
    value::Map* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Simulation sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);
        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","top");
                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_run: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run: empty result");

        resPython = pyvle_convert_dataframe(*res);
        delete(res);
        return resPython;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;

}

PyObject* pyvle_run_matrix(vpz::Vpz* file)
{
    assert(file);
    value::Map* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Simulation sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);
        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","none");
                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_run_matrix: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run_matrix: empty result");

        resPython = pyvle_convert_matrix(*res);
        delete res;
        return resPython;

    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_run_manager(vpz::Vpz* file)
{
    assert(file);
    value::Matrix* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Manager sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);

        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","top");
                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, 1, 0, 1, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_manager: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run_manager: empty result");

        resPython = pyvle_convert_simulation_dataframe(*res);
        delete res;
        return resPython;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_run_manager_matrix(vpz::Vpz* file)
{
    assert(file);

    value::Matrix* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Manager sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);

        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","none");
                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, 1, 0, 1, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_manager: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run_manager_matrix: empty result");

        resPython = pyvle_convert_simulation_matrix(*res);
        return resPython;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_run_manager_thread(vpz::Vpz* file, int th)
{
    assert(file);

    value::Matrix* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Manager sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);

        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","top");

                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, th, 0,1, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_manager_thread: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run_manager_thread: empty result");

        resPython = pyvle_convert_simulation_dataframe(*res);
        delete res;
        return resPython;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

PyObject* pyvle_run_manager_thread_matrix(vpz::Vpz* file, int th)
{
    assert(file);

    value::Matrix* res = NULL;
    PyObject* resPython = NULL;

    try {
        utils::ModuleManager man;
        manager::Error error;
        manager::Manager sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);

        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","none");
                output.setData(configOutput);
            }
        }

        res = sim.run(new vpz::Vpz(*file), man, th, 0,1, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_manager_thread: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }

        if (res == NULL)
            return PyString_FromString(
                "Error in pyvle_run_manager_thread_matrix: empty result");

        resPython = pyvle_convert_simulation_matrix(*res);
        return resPython;
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

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

void pyvle_condition_create(vle::vpz::Vpz* file,
        const std::string name)
{
    assert(file);

    vpz::Condition newCond(name);
    vpz::Conditions& listConditions(file->project().experiment().conditions());
    listConditions.add(newCond);
}

PyObject* pyvle_condition_size(vpz::Vpz* file)
{
    assert(file);

    return PyInt_FromLong(file->project().experiment().conditions().
            conditionlist().size());
}

PyObject* pyvle_condition_port_list_size(vpz::Vpz* file,
        std::string conditionname)
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

void pyvle_condition_port_clear(vpz::Vpz* file,
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

void pyvle_condition_add_boolean(vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        std::string value)
{
    assert(file);

    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));

    bool val;
    if(value=="true") {
        val=true;
    } else {
        val=false;
    }

    cnd.addValueToPort(portname, value::Boolean::create(val));
}

void pyvle_condition_add_value(vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        vle::value::Value* value)
{

    assert(file);

    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));

    cnd.addValueToPort(portname, *value);
}

void pyvle_condition_set_port_value(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        vle::value::Value* value,
        int i)
{
    assert(file);
    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& vector(cnd.getSetValues(portname).value());

    vector.at(i) = value->clone();
}

void pyvle_condition_set_value(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        std::string value,
        std::string type,
        int i)
{
    assert(file);

    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& vector(cnd.getSetValues(portname).value());

    if (type == "integer") {
        vector[i]=value::Integer::create(boost::lexical_cast<int> (value));
    } else if(type == "double") {
        vector[i]=value::Double::create(boost::lexical_cast<double> (value));
    } else if(type == "string") {
        vector[i]=value::String::create(value);
    } else if(type == "boolean") {
        bool val;
        if(value=="true") {
            val=true;
        } else {
            val=false;
        }
        vector[i]=value::Boolean::create(val);
    } else {
        vector[i]=value::String::create(value);
    }
}

PyObject* pyvle_condition_get_setvalue(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname)
{
    assert(file);

    PyObject* r;    /* condition list result */

    vle::vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& v(cnd.getSetValues(portname).value());
    int size = v.size();

    r = PyList_New(size);
    for (int i = 0; i < size; ++i) {
        PyList_SetItem(r, i, pyvle_convert_value(*v[i]));
    }
    return r;
}

PyObject* pyvle_condition_get_value(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        int i)
{
    assert(file);

    PyObject* r;

    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& vector(cnd.getSetValues(portname).value());

    r=pyvle_convert_value(*vector[i]);

    return r;
}

PyObject* pyvle_condition_get_value_type(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        int i)
{
    assert(file);

    PyObject* r;
    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& v(cnd.getSetValues(portname).value());

    switch(v[i]->getType()) {
    case vle::value::Value::DOUBLE: {
        r = PyString_FromString("double");
        break;
    }
    case vle::value::Value::INTEGER: {
        r = PyString_FromString("integer");
        break;
    }
    case vle::value::Value::STRING: {
        r = PyString_FromString("string");
        break;
    }
    case vle::value::Value::BOOLEAN: {
        r = PyString_FromString("boolean");
        break;
    }
    case vle::value::Value::MAP: {
        r = PyString_FromString("map");
        break;
    }
    case vle::value::Value::SET: {
        r = PyString_FromString("set");
        break;
    }
    case vle::value::Value::TUPLE : {
        r = PyString_FromString("tuple");
        break;
    }
    case vle::value::Value::TABLE : {
        r = PyString_FromString("table");
        break;
    }
    case vle::value::Value::XMLTYPE : {
        r = PyString_FromString("xml");
        break;
    }
    case vle::value::Value::MATRIX : {
        r = PyString_FromString("matrix");
        break;
    }
    case vle::value::Value::NIL : {
        r = PyString_FromString("none");
        break;
    }
    default : {
        r = PyString_FromString("none");
        break;
    }
    }

    return r;
}

void pyvle_condition_delete_value(vle::vpz::Vpz* file,
        std::string conditionname,
        std::string portname,
        int i)
{
    assert(file);

    vpz::Condition& cnd(file->project().experiment().
            conditions().get(conditionname));
    vle::value::VectorValue& vector(cnd.getSetValues(portname).value());
    vle::value::VectorValue::iterator it = vector.begin();

    value::Value* base = vector[i];

    while (it != vector.end()) {
        if (&**it == base)
            break;
        ++it;
    }

    if (it != vector.end()) {
        vector.erase(it);
    }

}

PyObject* pyvle_atomic_model_conditions_list(vle::vpz::Vpz* file,
        std::string name)
{
    assert(file);

    PyObject* r;
    std::vector < vpz::AtomicModel* > list;
    file->project().model().getAtomicModelList(list);

    std::vector < vpz::AtomicModel* >::const_iterator itb = list.begin();
    std::vector < vpz::AtomicModel* >::const_iterator ite = list.end();

    for(; itb != ite; itb ++){
        if ((*itb)->getName() == name) {
            std::vector < std::string > s = (*itb)->conditions();
            std::vector < std::string >::iterator sit;
            int size = s.size();
            r = PyList_New(size);
            int i = 0;
            for (sit = s.begin(); sit != s.end(); ++sit, ++i)
                PyList_SetItem(r, i, PyString_FromString(sit->c_str()));
            return r;
        }
    }
    return NULL;
}

PyObject* pyvle_dynamic_conditions_list(vle::vpz::Vpz* file,
        std::string name)
{
    assert(file);

    PyObject* r;
    std::vector < vpz::AtomicModel* > list;
    file->project().model().getAtomicModelList(list);

    std::vector < vpz::AtomicModel* >::const_iterator itb = list.begin();
    std::vector < vpz::AtomicModel* >::const_iterator ite = list.end();

    for(; itb != ite; itb ++){
        if ((*itb)->dynamics() == name) {
            std::vector < std::string > s = (*itb)->conditions();
            std::vector < std::string >::iterator sit;
            int size = s.size();
            r = PyList_New(size);
            int i = 0;
            for (sit = s.begin(); sit != s.end(); ++sit, ++i)
                PyList_SetItem(r, i, PyString_FromString(sit->c_str()));
            return r;
        }
    }
    return NULL;
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

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

PyObject* pyvle_dynamic_get_model_list(vle::vpz::Vpz* file,
        std::string dynamicname)
{
    assert(file);

    PyObject* r;
    r = PyList_New(0);
    std::vector < vpz::AtomicModel* > list;
    file->project().model().getAtomicModelList(list);

    std::vector < vpz::AtomicModel* >::const_iterator itb = list.begin();
    std::vector < vpz::AtomicModel* >::const_iterator ite = list.end();

    for(; itb != ite; itb ++){
        if ((*itb)->dynamics() == dynamicname) {
            PyList_Append(r, PyString_FromString((*itb)->getName().c_str()));
        }
    }
    return r;
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

PyObject* pyvle_list_view_entries(vle::vpz::Vpz* file)
{
    //key = name of a view, value = list of complete port names
    typedef std::map<std::string, std::list<std::string> > ViewEntries;
    ViewEntries viewEntries;

    std::vector < vpz::AtomicModel* > list;
    file->project().model().getAtomicModelList(list);

    std::vector < vpz::AtomicModel* >::const_iterator itb = list.begin();
    std::vector < vpz::AtomicModel* >::const_iterator ite = list.end();


    const vle::vpz::Observables& observables=
            file->project().experiment().views().observables();

    for(;itb!=ite;itb++){
        const vle::vpz::AtomicModel& atom = **itb;
        const std::string& obs = atom.observables();
        if(obs != ""){
            const vpz::ObservablePortList& obsportlst =
                    observables.get(obs).observableportlist();
            vpz::ObservablePortList::const_iterator ibo = obsportlst.begin() ;
            vpz::ObservablePortList::const_iterator ieo = obsportlst.end();
            for(;ieo!=ibo;ibo++){
                const vpz::ObservablePort& port =
                        observables.get(obs).get(ibo->first);
                vpz::ViewNameList list = port.viewnamelist();
                vpz::ViewNameList::iterator ibl = list.begin();
                vpz::ViewNameList::iterator iel = list.end();
                for(;ibl!=iel;ibl++){
                    std::list<std::string>& ll = viewEntries[*ibl];
                    std::stringstream ss;
                    ss << atom.getCompleteName() << "." << ibo->first;
                    ll.push_back(ss.str());
                }
            }
        }
    }
    PyObject* result = PyDict_New();
    ViewEntries::iterator ibv = viewEntries.begin();
    ViewEntries::iterator iev = viewEntries.end();
    for (;ibv!=iev;ibv++) {
        std::list<std::string>& completePorts = ibv->second;
        unsigned int size = completePorts.size();
        PyObject* r = PyList_New(completePorts.size());
        std::list<std::string>::iterator ibp = completePorts.begin();
        std::list<std::string>::iterator iep = completePorts.end();
        for(unsigned int i =0; i<size; i++, ibp++){
            PyList_SetItem(r, i, PyString_FromString(ibp->c_str()));
        }
        PyDict_SetItem(result, PyString_FromString(ibv->first.c_str()),r);
    }
    return result;
}

PyObject* pyvle_get_output_plugin(vle::vpz::Vpz* file,
        std::string outputname)
{
    assert(file);

    PyObject* r;
    vpz::Output& out(file->project().experiment().views().outputs().
            get(outputname));

    r = PyString_FromString(out.plugin().c_str());
    return r;
}

PyObject* pyvle_observables_list(vle::vpz::Vpz* file)
{
    assert(file);

    PyObject* r;    /* observables list result */
    vpz::ObservableList& obslst(file->project().experiment().views().
            observables().observablelist());
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

PyObject* pyvle_outputs_list(vle::vpz::Vpz* file)
{
    assert(file);

    PyObject* r;    /* outputs list result */
    vpz::OutputList& lst(file->project().experiment().views().outputs().
            outputlist());
    vpz::OutputList::iterator it;
    int size;
    int i;

    size = lst.size();

    r = PyList_New(size);
    i = 0;

    if (size > 0) {
        for (it = lst.begin(); it != lst.end(); ++it, ++i) {
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

    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));

    if (!obs.exist(portname)) {
        obs.add(portname);
    }
}

void pyvle_observable_del_port(vle::vpz::Vpz* file,
        std::string obsname,
        std::string portname)
{
    assert(file);

    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));

    if (obs.exist(portname)) {
        obs.del(portname);
    }
}

PyObject* pyvle_observable_has_view(vle::vpz::Vpz* file,
        std::string obsname,
        std::string viewname)
{
    assert(file);

    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));

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
    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));
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

    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));

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

    vpz::Observable& obs(file->project().experiment().views().observables().
            get(obsname));

    obs.permanent(ispermanent);
}

PyObject* pyvle_observable_port_attached_views(vle::vpz::Vpz* file,
        std::string obsname,
        std::string portname)
{
    assert(file);

    PyObject* r;
    vpz::ObservablePort& port(file->project().experiment().views().
            observables().get(obsname).get(portname));
    vpz::ViewNameList list = port.viewnamelist();
    vpz::ViewNameList::iterator it;

    int size;
    int i;

    size = list.size();

    r = PyList_New(size);
    i = 0;

    if (size > 0) {
        for (it = list.begin(); it != list.end(); ++it, ++i) {
            PyList_SetItem(r, i, PyString_FromString(it->c_str()));
        }
    }
    return r;

}

PyObject* pyvle_dynamic_observables_list(vle::vpz::Vpz* file,
        std::string name)
{
    assert(file);
    PyObject* r = NULL;
    std::vector < vpz::AtomicModel* > list;
    file->project().model().getAtomicModelList(list);

    std::vector < vpz::AtomicModel* >::const_iterator itb = list.begin();
    std::vector < vpz::AtomicModel* >::const_iterator ite = list.end();

    for(; itb != ite; itb ++){
        const vpz::AtomicModel& atom = **itb;
        if(atom.getName() == name){
            std::string s = atom.observables();
            r = PyString_FromString(s.c_str());
            return r;
        }

    }
    return PyString_FromString("dynamic not found");
}

PyObject* pyvle_nbreplicas(vle::vpz::Vpz* file)
{
    assert(file);

    PyObject* r;
    // int nb = file->project().experiment().replicas().number();
    //    r = PyInt_FromLong(nb);
    //TODO
    r = PyInt_FromLong(1);
    return r;
}

struct cond_t {
    cond_t() : sz(0), pos(0) { }

    size_t  sz;
    size_t  pos;
};

PyObject* pyvle_combinations(vle::vpz::Vpz* file)
{
    assert(file);

    PyObject* r;

    std::vector < cond_t > conditions;
    const vpz::Experiment& exp = file->project().experiment();
    const vpz::Conditions& cnds = exp.conditions();
    vpz::ConditionList::const_iterator it;

    for (it = cnds.conditionlist().begin();
            it != cnds.conditionlist().end(); ++it) {
        const vpz::Condition& cnd = it->second;
        vpz::ConditionValues::const_iterator jt;

        if (not cnd.conditionvalues().empty()) {
            for (jt = cnd.conditionvalues().begin();
                    jt != cnd.conditionvalues().end(); ++jt) {
                conditions.push_back(cond_t());

                conditions[conditions.size() - 1].sz = jt->second->size();
                conditions[conditions.size() - 1].pos = 0;
            }
        } else {
            conditions.push_back(cond_t());
            conditions[conditions.size() - 1].sz = 1;
            conditions[conditions.size() - 1].pos = 0;
        }
    }

    size_t combinationNumber = 1;

    if (exp.combination() == "linear") {
        std::vector < cond_t >::const_iterator it;
        for (it = conditions.begin(); it != conditions.end(); ++it) {
            if (it->sz != 1) {
                combinationNumber = it->sz;
                break;
            }
        }
    } else {
        std::vector < cond_t >::const_iterator it;
        for (it = conditions.begin(); it != conditions.end(); ++it) {
            combinationNumber *= it->sz;
        }
    }

    r = PyList_New(combinationNumber);

    size_t nb = 0;

    do {

        PyObject* l = PyList_New(conditions.size());

        PyList_SetItem(r, nb, l);

        vpz::ConditionList::const_iterator itOrig =
                cnds.conditionlist().begin();
        vpz::ConditionValues::const_iterator
        itValueOrig = itOrig->second.conditionvalues().begin();

        for (size_t jcom = 0; jcom < conditions.size(); ++jcom) {
            if (not itOrig->second.conditionvalues().empty()) {
                size_t index = conditions[jcom].pos;
                value::Value* val = itValueOrig->second->get(index);

                PyList_SetItem(l, jcom, PyString_FromString(
                        val->writeToString().c_str()));

                itValueOrig++;
                if (itValueOrig == itOrig->second.conditionvalues().end()) {
                    itOrig++;
                    itValueOrig = itOrig->second.conditionvalues().begin();
                }
            }
        }
        if (exp.combination() == "linear") {
            for(size_t i=0; i < conditions.size(); ++i)
                if (conditions[i].sz != 1)
                    conditions[i].pos++;
        } else {
            size_t sz = conditions.size() - 1;

            if (conditions[sz].pos != conditions[sz].sz - 1) {
                conditions[sz].pos++;
            } else {
                int i = sz;

                while (i >= 0) {
                    if (conditions[i].pos == conditions[i].sz - 1) {
                        conditions[i].pos = 0;
                        i--;
                    } else {
                        conditions[i].pos++;
                        break;
                    }
                }
            }
        }
        ++nb;
    } while (nb < combinationNumber);

    return r;
}

PyObject* pyvle_experiment_get_name(vle::vpz::Vpz* file)
{
    assert(file);

    PyObject* r;

    std::string expname(file->project().experiment().name());
    r = PyString_FromString(expname.c_str());

    return r;
}

PyObject* pyvle_trace_run_error(vle::vpz::Vpz* file)
{
    std::string m_out;
    bool m_error = false;
    utils::ModuleManager m_modulemgr;
    devs::RootCoordinator m_root(m_modulemgr);
    try {
        {
            m_out+=" - Open file.....................: ";
            vpz::Vpz vpz(*file);
            m_out+="ok<br/>";

            m_out+=" - Coordinator load models ......: ";
            m_root.load(vpz);
            m_out+="ok<br/>";

            m_out+=" - Clean project file ...........: ";
        }
        m_out+="ok<br/>";

        m_out+=" - Coordinator initializing .....: ";

        m_out+="ok<br/>";

        m_out+=" - Simulation run................: ";
        while (m_root.run()) {}
        m_out +="ok<br/>";

        m_out+=" - Coordinator cleaning .........: ";
        m_root.finish();
        m_out+="ok<br/>";
    } catch(const std::exception& e) {
        m_out+="<br/>/!\\ vle error reported: " +
                utils::demangle(typeid(e)) + "<br/>" + e.what();
        m_error = true;
    } catch(const Glib::Exception& e) {
        m_out+="<br/>/!\\ vle Glib error reported: " +
                utils::demangle(typeid(e)) + "<br/>" + e.what();
        m_error = true;
    }
    return PyString_FromString(m_out.c_str());
}

PyObject* pyvle_get_installed_packages()
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }

    PyObject* r;
    utils::PathList list = utils::Path::path().getBinaryPackages();
    utils::PathList::const_iterator it = list.begin();
    int i = 0;

    r = PyList_New(list.size());
    while (it != list.end()) {
        PyList_SetItem(r, i,
                PyString_FromString(
                        boost::filesystem::basename(*it).c_str()));
        ++it;
        ++i;
    }
    return r;
}

PyObject* pyvle_get_package_vpz_list(std::string name)
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }

    PyObject* r;
    utils::Package pack(name);
    utils::PathList list = pack.getExperiments();
    utils::PathList::const_iterator it = list.begin();
    int i = 0;

    r = PyList_New(list.size());
    while (it != list.end()) {
        PyList_SetItem(r, i,
                PyString_FromString(
                        boost::filesystem::basename(*it).c_str()));
        ++it;
        ++i;
    }
    return r;
}

PyObject* pyvle_get_package_vpz_directory(std::string name)
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }

    PyObject* r;

    utils::Package pack(name);

    r = PyString_FromString(pack.getExpDir().c_str());

    return r;
}

PyObject* pyvle_get_package_data_directory(std::string name)
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }

    PyObject* r;

    utils::Package pack(name);

    r = PyString_FromString(pack.getDataDir().c_str());

    return r;
}

PyObject* pyvle_get_package_output_directory(std::string name)
{
    if (!thread_init) {
        vle::Init app;//TODO
        thread_init = true;
    }

    PyObject* r;

    utils::Package pack(name);

    r = PyString_FromString(pack.getOutputDir().c_str());

    return r;
}

PyObject* pyvle_get_package_vpz(std::string name, std::string vpz)
{
    PyObject* r;

    utils::Package pack(name);
    r = PyString_FromString(pack.getExpFile(vpz).c_str());


    return r;
}

void pyvle_set_package_mode(std::string name)
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }
}

void pyvle_set_normal_mode()
{
    if (!thread_init) {
        vle::Init app;
        thread_init = true;
    }
}

void pyvle_set_output_plugin(vle::vpz::Vpz* file,
        std::string outputname,
        std::string location,
        std::string format,
        std::string plugin,
        std::string package)
{
    assert(file);

    vpz::Output& out(file->project().experiment().views().outputs().
            get(outputname));
    if (format == "local")
        out.setLocalStream(location, plugin, package);
    else
        out.setDistantStream(location, plugin, package);
}

PyObject* pyvle_get_output_format(vle::vpz::Vpz* file,
        std::string outputname)
{
    assert(file);

    vpz::Output& out(file->project().experiment().views().outputs().
            get(outputname));
    return PyString_FromString(out.streamformat().c_str());
}

PyObject* pyvle_get_output_location(vle::vpz::Vpz* file,
        std::string outputname)
{
    assert(file);

    vpz::Output& out(file->project().experiment().views().outputs().
            get(outputname));
    return PyString_FromString(out.location().c_str());
}

PyObject* pyvle_run_combination(vle::vpz::Vpz* file, int comb)
{
    assert(file);

    vpz::Vpz tmp_file(*file);

    std::vector < cond_t > conditions;
    const vpz::Experiment& exp = tmp_file.project().experiment();
    const vpz::Conditions& cnds = exp.conditions();
    const vpz::Experiment& orig_exp = file->project().experiment();
    const vpz::Conditions& orig_cnds = orig_exp.conditions();
    vpz::ConditionList::const_iterator it;

    for (it = cnds.conditionlist().begin();
            it != cnds.conditionlist().end(); ++it) {
        const vpz::Condition& cnd = it->second;
        vpz::ConditionValues::const_iterator jt;

        if (not cnd.conditionvalues().empty()) {
            for (jt = cnd.conditionvalues().begin();
                    jt != cnd.conditionvalues().end(); ++jt) {
                conditions.push_back(cond_t());

                conditions[conditions.size() - 1].sz = jt->second->size();
                conditions[conditions.size() - 1].pos = 0;
            }
        } else {
            conditions.push_back(cond_t());
            conditions[conditions.size() - 1].sz = 1;
            conditions[conditions.size() - 1].pos = 0;
        }
    }

    size_t combinationNumber = 1;

    if (exp.combination() == "linear") {
        std::vector < cond_t >::const_iterator it;
        for (it = conditions.begin(); it != conditions.end(); ++it) {
            if (it->sz != 1) {
                combinationNumber = it->sz;
                break;
            }
        }
    } else {
        std::vector < cond_t >::const_iterator it;
        for (it = conditions.begin(); it != conditions.end(); ++it) {
            combinationNumber *= it->sz;
        }
    }

    size_t nb = 0;

    vpz::ConditionList::const_iterator itOrig =
            cnds.conditionlist().begin();
    vpz::ConditionList::const_iterator itOrig_o =
            orig_cnds.conditionlist().begin();
    vpz::ConditionValues::const_iterator
    itValueOrig = itOrig->second.conditionvalues().begin();
    vpz::ConditionValues::const_iterator
    itValueOrig_o = itOrig_o->second.conditionvalues().begin();

    //Emptying tmpfile condvalues
    for (size_t jcom = 0; jcom < conditions.size(); ++jcom) {
        if (not itOrig->second.conditionvalues().empty()) {
            size_t index = conditions[jcom].pos;
            itValueOrig->second->clear();
            itValueOrig++;
            if (itValueOrig == itOrig->second.conditionvalues().end()) {
                itOrig++;
                itValueOrig = itOrig->second.conditionvalues().begin();
            }
        }
    }

    do {
        itOrig = cnds.conditionlist().begin();
        itValueOrig = itOrig->second.conditionvalues().begin();
        itOrig_o = orig_cnds.conditionlist().begin();
        itValueOrig_o = itOrig_o->second.conditionvalues().begin();

        for (size_t jcom = 0; jcom < conditions.size(); ++jcom) {
            if (not itOrig_o->second.conditionvalues().empty()) {
                size_t index = conditions[jcom].pos;
                if (nb == comb) {
                    value::Value* val = itValueOrig_o->second->get(index);
                    itValueOrig->second->add(val);
                }
                itValueOrig_o++;
                itValueOrig++;
                if (itValueOrig == itOrig->second.conditionvalues().end()) {
                    itOrig++;
                    itValueOrig = itOrig->second.conditionvalues().begin();
                }
                if (itValueOrig_o == itOrig_o->second.conditionvalues().end()) {
                    itOrig_o++;
                    itValueOrig_o = itOrig_o->second.conditionvalues().begin();
                }
            }
        }

        if (exp.combination() == "linear") {
            for(size_t i=0; i < conditions.size(); ++i)
                if (conditions[i].sz != 1)
                    conditions[i].pos++;
        } else {
            size_t sz = conditions.size() - 1;

            if (conditions[sz].pos != conditions[sz].sz - 1) {
                conditions[sz].pos++;
            } else {
                int i = sz;

                while (i >= 0) {
                    if (conditions[i].pos == conditions[i].sz - 1) {
                        conditions[i].pos = 0;
                        i--;
                    } else {
                        conditions[i].pos++;
                        break;
                    }
                }
            }

        }
        ++nb;
    } while (nb < combinationNumber);

    try {
        utils::ModuleManager man;
        manager::Simulation sim(manager::LOG_NONE,
                manager::SIMULATION_NONE,
                NULL);
        manager::Error error;

        //configure output plugins for column names
        vpz::Outputs::iterator itb =
                file->project().experiment().views().outputs().begin();
        vpz::Outputs::iterator ite =
                file->project().experiment().views().outputs().end();
        for(;itb!=ite;itb++) {
            vpz::Output& output = itb->second;
            if((output.package() == "vle.output") &&
                    (output.plugin() == "storage")){
                value::Map* configOutput = new value::Map();
                configOutput->addString("header","top");
                output.setData(configOutput);
            }
        }

        value::Map* res = sim.run(new vpz::Vpz(tmp_file), man, &error);

        if (error.code != 0) {
            std::string filename = utils::Trace::getLogFilename("pyvle.log");
            std::ofstream* logfile = new std::ofstream(filename.c_str());
            (*logfile) << _("Error in pyvle_run: ")
                            << error.message
                            << "\n\n" << std::flush;
            logfile->close();
            return PyString_FromString(error.message.c_str());
        }
        return pyvle_convert_dataframe(*res);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

void pyvle_set_nbreplicas(vle::vpz::Vpz* file,
        int /*number*/)
{
    assert(file);

    //file->project().experiment().replicas().setNumber(number)
    //TODO
}

PyObject* pyvle_get_seedreplicas(vle::vpz::Vpz* file)
{
    assert(file);

    //long seed = file->project().experiment().replicas().seed();
    //    return PyInt_FromLong(seed);
    //TODO
    return PyInt_FromLong(1);
}

void pyvle_set_seedreplicas(vle::vpz::Vpz* file,
        long /*number*/)
{
    assert(file);

    //file->project().experiment().replicas().setSeed(number);
    //TODO
}

/*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

vle::value::Value* pyvle_create_map()
{
    return vle::value::Map::create();
}

vle::value::Value* pyvle_create_set()
{
    return vle::value::Set::create();
}

vle::value::Value* pyvle_create_matrix(unsigned int width, unsigned int height)
{
    return vle::value::Matrix::create(width, height);
}

vle::value::Value* pyvle_create_table(unsigned int width, unsigned int height)
{
    return vle::value::Table::create(width, height);
}

vle::value::Value* pyvle_create_tuple(unsigned int size)
{
    return vle::value::Tuple::create(size,0);
}

vle::value::Value* pyvle_int_to_value(long i)
{
    return vle::value::Integer::create(i);
}

vle::value::Value* pyvle_real_to_value(float i)
{
    return vle::value::Double::create(i);
}

vle::value::Value* pyvle_str_to_xml(std::string i)
{
    return vle::value::Xml::create(i);
}


vle::value::Value* pyvle_string_to_value(std::string i)
{
    return vle::value::String::create(i);
}

vle::value::Value* pyvle_bool_to_value(bool i)
{
    return vle::value::Boolean::create(i);
}

void pyvle_add_value_to_map(vle::value::Value* map, std::string key,
        vle::value::Value* val)
{
    map->toMap().add(key, val->clone());
}

void pyvle_set_value_to_table(vle::value::Value* table, unsigned int i,
        unsigned int j, double v)
{
    table->toTable().value()[i][j] = v;
}

void pyvle_set_value_to_matrix(vle::value::Value* mat, unsigned int i,
        unsigned int j, vle::value::Value* v)
{
    mat->toMatrix().set(i,j,v->clone());
}

void pyvle_set_value_to_tuple(vle::value::Value* tuple, unsigned int i, double v)
{
    tuple->toTuple().value()[i] = v;
}

void pyvle_add_value_to_set(vle::value::Value* set, vle::value::Value* val)
{
    set->toSet().add(val->clone());
}

void pyvle_compileTestPackages()
{
    namespace vu = vle::utils;

    std::string filename = vu::Trace::getLogFilename("pyvle.log");
    std::ofstream* logfile = new std::ofstream(filename.c_str());

    (*logfile) << _("pyvle_compileTestPackages")
            << "\n\n" << std::flush;

    try {
        //vlehome dir is set before calling this method
        vu::Package pack("vle.output");
        pack.configure();
        pack.wait((*logfile), (*logfile));
        if (pack.isSuccess()) {
            pack.build();
            pack.wait((*logfile), (*logfile));
            if (pack.isSuccess()) {
                pack.install();
                pack.wait((*logfile), (*logfile));
            }
        }
        pack.select("test_port");
        pack.configure();
        pack.wait((*logfile), (*logfile));
        if (pack.isSuccess()) {
            pack.build();
            pack.wait((*logfile), (*logfile));
            if (pack.isSuccess()) {
                pack.install();
                pack.wait((*logfile), (*logfile));
            }
        }
    }  catch(const std::exception& e) {
        (*logfile) << _("Error while compiling test_port and vle.output : \n")
                <<  e.what() << "\n\n" << std::flush;
    }
    logfile->close();
}
