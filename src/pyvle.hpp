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
void pyvle_condition_set_value(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				std::string value,
				std::string type,
				int i);
PyObject* pyvle_condition_get_setvalue(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname);
PyObject* pyvle_condition_get_value(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				int i);
PyObject* pyvle_condition_get_value_type(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				int i);
void pyvle_condition_delete_value(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				int i);
PyObject* pyvle_atomic_model_conditions_list(vle::vpz::Vpz* file,
		std::string name);
PyObject* pyvle_dynamic_conditions_list(vle::vpz::Vpz* file,
		std::string name);
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
PyObject* pyvle_views_list(vle::vpz::Vpz* file);
PyObject* pyvle_view_get_name(vle::vpz::Vpz* file,
				std::string viewname);
PyObject* pyvle_view_get_type(vle::vpz::Vpz* file,
				std::string viewname);
PyObject* pyvle_view_get_timestep(vle::vpz::Vpz* file,
				std::string viewname);
PyObject* pyvle_view_get_output(vle::vpz::Vpz* file,
				std::string viewname);
PyObject* pyvle_view_get_data(vle::vpz::Vpz* file,
				std::string viewname);
void pyvle_view_set_name(vle::vpz::Vpz* file,
				std::string viewoldname,
				std::string viewnewname);
void pyvle_view_set_type(vle::vpz::Vpz* file,
				std::string viewname,
				std::string viewtype);
void pyvle_view_set_timestep(vle::vpz::Vpz* file,
				std::string viewname,
				double time);
void pyvle_view_set_data(vle::vpz::Vpz* file,
				std::string viewname,
				std::string data);
void pyvle_views_add_eventview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output);
void pyvle_views_add_timedview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output,
				double time);
void pyvle_views_add_finishview(vle::vpz::Vpz* file,
				std::string viewname,
				std::string output);
PyObject* pyvle_get_output_plugin(vle::vpz::Vpz* file,
				std::string outputname);
PyObject* pyvle_observables_list(vle::vpz::Vpz* file);
void pyvle_observable_add(vle::vpz::Vpz* file,
				std::string obsname);
void pyvle_observable_del(vle::vpz::Vpz* file,
				std::string obsname);
PyObject* pyvle_observable_exists(vle::vpz::Vpz* file,
				std::string obsname);
void pyvle_observables_clear(vle::vpz::Vpz* file);
PyObject* pyvle_observables_empty(vle::vpz::Vpz* file);
PyObject* pyvle_observable_get_name(vle::vpz::Vpz* file,
				std::string obsname);
PyObject* pyvle_observable_ports_list(vle::vpz::Vpz* file,
				std::string obsname);
void pyvle_observable_add_port(vle::vpz::Vpz* file,
				std::string obsname,
				std::string portname);
void pyvle_observable_del_port(vle::vpz::Vpz* file,
				std::string obsname,
				std::string portname);
PyObject* pyvle_observable_has_view(vle::vpz::Vpz* file,
				std::string obsname,
				std::string viewname);
PyObject* pyvle_observable_get_port_name(vle::vpz::Vpz* file,
				std::string obsname,
				std::string viewname);
PyObject* pyvle_observable_is_permanent(vle::vpz::Vpz* file,
				std::string obsname);
void pyvle_observable_set_permanent(vle::vpz::Vpz* file,
				std::string obsname,
				bool ispermanent);
PyObject* pyvle_dynamic_observables_list(vle::vpz::Vpz* file,
				std::string name);
PyObject* pyvle_export(vle::vpz::Vpz* file,
		std::string location,
		std::string view,
		std::string type);
#endif
