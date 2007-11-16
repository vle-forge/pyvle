/** 
 * @file rvle.cpp
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

#include "rvle.h"
#include <vle/vpz.hpp>
#include <vle/manager.hpp>
#include <cassert>

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
#ifndef NDEBUG
    std::cerr << "rvle_open(" << filename << ")\n";
#endif
    vle::vpz::Vpz*  file = 0;

    try {
        file = new vle::vpz::Vpz(filename);
        return file;
    } catch(const std::exception& e) {
#ifndef NDEBUG
        std::cerr << "open error: " << e.what() << "\n";
#endif
        return 0;
    }
}

int rvle_run(RVLE handle)
{
#ifndef NDEBUG
    std::cerr << "rvle_run(" << handle << ")\n";
#endif
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    try {
        vle::manager::Simulator::run(*file);
    } catch(const std::exception& e) {
#ifndef NDEBUG
        std::cerr << "run error: " << e.what() << "\n";
#endif
        return 0;
    }
#ifndef NDEBUG
    std::cerr << "end rvle_run(" << handle << ")\n";
#endif
    return -1;
}

int rvle_close(RVLE handle)
{
#ifndef NDEBUG
    std::cerr << "rvle_close(" << handle << ")\n";
#endif
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    file->clear();
    return -1;
}

int rvle_delete(RVLE handle)
{
#ifndef NDEBUG
    std::cerr << "rvle_delete(" << handle << ")\n";
#endif
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    delete file;
    return -1;
}

char** rvle_condition_list(RVLE handle)
{
#ifndef NDEBUG
    std::cerr << "rvle_condition_list(" << handle << ")\n";
#endif
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    std::list < std::string > lst;

    file->project().experiment().conditions().conditionnames(lst);
    int maxsize = max_string_size(lst);

    char** result;

    result = (char**)malloc(lst.size() * sizeof(char*));
    std::list < std::string >::iterator it = lst.begin();

    for (size_t i = 0; i < lst.size(); ++i) {
        result[i] = (char*)malloc(maxsize * sizeof(char));
        strcpy(result[i], (*it).c_str());
        it++;
    }

    return result;
}

int rvle_condition_size(RVLE handle)
{
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    return file->project().experiment().conditions().conditionlist().size();
}
