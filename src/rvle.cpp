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

RVLE rvle_new(const char* filename)
{
    vle::vpz::Vpz*  file;

    try {
        new vle::vpz::Vpz(filename);
        return file;
    } catch(const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 0;
    }
}

void rvle_run(RVLE handle)
{
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    vle::manager::Simulator::run(*file);
}

void rvle_close(RVLE handle)
{
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    file->clear();
}

void rvle_delete(RVLE handle)
{
    vle::vpz::Vpz*  file(reinterpret_cast < vle::vpz::Vpz* >(handle));
    delete file;
}
