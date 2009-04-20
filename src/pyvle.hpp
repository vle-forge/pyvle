/**
 * @file        pyvle.hpp
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

#ifndef __PYVLE_HPP
#define __PYVLE_HPP

#include <Python.h>
#include <vle/vpz.hpp>

vle::vpz::Vpz* pyvle_open(const char* filename);
void pyvle_delete(vle::vpz::Vpz* file);
PyObject* pyvle_run(vle::vpz::Vpz* file);
PyObject* pyvle_run_matrix(vle::vpz::Vpz* file);
PyObject* pyvle_manager(vle::vpz::Vpz* file);
PyObject* pyvle_manager_matrix(vle::vpz::Vpz* file);
PyObject* pyvle_manager_thread(vle::vpz::Vpz* file, int th);
PyObject* pyvle_manager_thread_matrix(vle::vpz::Vpz* file, int th);
PyObject* pyvle_manager_cluster(vle::vpz::Vpz* file);
PyObject* pyvle_manager_cluster_matrix(vle::vpz::Vpz* file);
PyObject* pyvle_condition_size(vle::vpz::Vpz* file);
PyObject* pyvle_condition_list(vle::vpz::Vpz* file);
PyObject* pyvle_condition_show(vle::vpz::Vpz* file,
			       std::string conditionname,
			       std::string portname);
PyObject* pyvle_condition_port_list_size(vle::vpz::Vpz* file,
					 std::string conditionname);
PyObject* pyvle_condition_port_list(vle::vpz::Vpz* file,
				    std::string conditionname);
void pyvle_condition_clear(vle::vpz::Vpz* file,
			   std::string conditionname,
			   std::string portname);
void pyvle_condition_add_real(vle::vpz::Vpz* file,
			      std::string conditionname,
			      std::string portname,
			      double value);
void pyvle_condition_add_integer(vle::vpz::Vpz* file,
				 std::string conditionname,
				 std::string portname,
				 long value);
void pyvle_condition_add_string(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				std::string value);
void pyvle_experiment_set_duration(vle::vpz::Vpz* file,
				   double value);
PyObject* pyvle_experiment_get_duration(vle::vpz::Vpz* file);
void pyvle_experiment_set_seed(vle::vpz::Vpz* file,
				   double value);
PyObject* pyvle_experiment_get_seed(vle::vpz::Vpz* file);
void pyvle_save(vle::vpz::Vpz* file,
		std::string filename);
PyObject* pyvle_dynamics_list(vle::vpz::Vpz* file);
PyObject* pyvle_dynamic_get_name(vle::vpz::Vpz* file,
				std::string dynamicname);
PyObject* pyvle_dynamic_get_model(vle::vpz::Vpz* file,
				std::string dynamicname);
PyObject* pyvle_dynamic_get_library(vle::vpz::Vpz* file,
				std::string dynamicname);
PyObject* pyvle_dynamic_get_language(vle::vpz::Vpz* file,
				std::string dynamicname);
void pyvle_dynamic_set_model(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string model);
void pyvle_dynamic_set_library(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string library);
void pyvle_dynamic_set_language(vle::vpz::Vpz* file,
				std::string dynamicname,
				std::string language);
#endif
