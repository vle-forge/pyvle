/** 
 * @file rvle.cpp
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



#include "rvle.h"
#include <vle/vpz.hpp>
#include <vle/manager.hpp>
#include <vle/value.hpp>
#include <cassert>

using namespace vle;

//
// C++ utilities
//


int max_string_size(const std::list < std::string >& lst)
{
    assert(not lst.empty());

    int r = 0;
    std::list < std::string >::const_iterator it;
    for (it = lst.begin(); it != lst.end(); ++it) {
        if ((*it).size() > r) {
            r = (*it).size();
        }
    }

    assert(r > 0);

    return r;
}

//
// R interface
//

RVLE rvle_open(const char* filename)
{
    assert(filename);

    vpz::Vpz*  file = 0;

    try {
        file = new vpz::Vpz(filename);
        return file;
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_run(RVLE handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        manager::RunVerbose jrm(std::cerr);
        jrm.start(*file);
    } catch(const std::exception& e) {
        return 0;
    }
    return -1;
}

int rvle_delete(RVLE handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    delete file;
    return -1;
}

char** rvle_condition_list(RVLE handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    std::list < std::string > lst;

    file->project().experiment().conditions().conditionnames(lst);
    assert(lst.size() > 0);

    char** result;

    result = (char**)malloc(lst.size() * sizeof(char*));
    std::list < std::string >::iterator it = lst.begin();

    for (size_t i = 0; i < lst.size(); ++i) {
        result[i] = (char*)malloc((*it).size() + 1);
        strcpy(result[i], (*it).c_str());
        it++;
    }

    return result;
}

char** rvle_condition_port_list(RVLE handle, const char* conditionname)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    std::list < std::string > lst;
    char** result;

    try {
        const vpz::Condition& cnd(
            file->project().experiment().conditions().get(conditionname));

        cnd.portnames(lst);

        result = (char**)malloc(lst.size() * sizeof(char*));
        std::list < std::string >::iterator it = lst.begin();

        for (size_t i = 0; i < lst.size(); ++i) {
            result[i] = (char*)malloc((*it).size() + 1);
            strcpy(result[i], (*it).c_str());
            it++;
        }
    } catch(const std::exception& e) {
        return 0;
    }

    return result;
}

int rvle_condition_port_list_size(RVLE handle, const char* conditionname)
{
    assert(handle);

    int result;

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        vpz::Condition& cnd(
            file->project().experiment().conditions().get(conditionname));
        result = cnd.conditionvalues().size();
    } catch(const std::exception& e) {
        result = 0;
    }

    return result;
}

int rvle_condition_size(RVLE handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    return file->project().experiment().conditions().conditionlist().size();
}

int rvle_condition_set_real(RVLE handle,
                            const char* conditionname,
                            const char* portname,
                            double value)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.setValueToPort(portname, value::DoubleFactory::create(value));
    } catch(const std::exception& e) {
        return 0;
    }

    return -1;
}

int rvle_condition_set_integer(RVLE handle,
                            const char* conditionname,
                            const char* portname,
                            long value)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.setValueToPort(portname, value::IntegerFactory::create(value));
    } catch(const std::exception& e) {
        return 0;
    }

    return -1;
}


int rvle_save(RVLE handle, const char* filename)
{
    assert(handle and filename);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        file->write(filename);
    } catch(const std::exception& e) {
        return 0;
    }

    return -1;
}
