/*
 * @file src/pyvle.hpp
 *
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems
 * http://www.vle-project.org
 *
 * Copyright (c) 2003-2007 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (c) 2003-2012 ULCO http://www.univ-littoral.fr
 * Copyright (c) 2007-2012 INRA http://www.inra.fr
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


#ifndef VLE_PORT_PYVLE_HPP
#define VLE_PORT_PYVLE_HPP 1

#include <Python.h>
#include <vle/vpz.hpp>

vle::vpz::Vpz* pyvle_open(const char* filename);
vle::vpz::Vpz* pyvle_open_pkg(const char* pkgname, const char* filename);
vle::vpz::Vpz* pyvle_from_buffer(const std::string& buffer);
vle::vpz::Vpz* pyvle_from_buffer_pkg(const char* pkgname,
                                     const std::string& buffer);
void pyvle_save(vle::vpz::Vpz* file,
		std::string filename);
PyObject* pyvle_save_buffer(vle::vpz::Vpz* file);
void pyvle_delete(vle::vpz::Vpz* file);

/* name of experiments */
void pyvle_experiment_set_name(vle::vpz::Vpz* file,
				   const std::string& name);

/* begin of experiments */
void pyvle_experiment_set_begin(vle::vpz::Vpz* file,
				   double value);
PyObject* pyvle_experiment_get_begin(vle::vpz::Vpz* file);

/* duration of experiments */
void pyvle_experiment_set_duration(vle::vpz::Vpz* file,
				   double value);
PyObject* pyvle_experiment_get_duration(vle::vpz::Vpz* file);

/* seed of experiments */
void pyvle_experiment_set_seed(vle::vpz::Vpz* file,
			       double value);
PyObject* pyvle_experiment_get_seed(vle::vpz::Vpz* file);

/* replicas */
void pyvle_experiment_set_linear_combination(vle::vpz::Vpz* file,
					     int seed, int replicas);
void pyvle_experiment_set_total_combination(vle::vpz::Vpz* file,
					    int seed, int replicas);

/* execution of experiments */
PyObject* pyvle_run(vle::vpz::Vpz* file);
PyObject* pyvle_run_matrix(vle::vpz::Vpz* file);
PyObject* pyvle_run_manager(vle::vpz::Vpz* file);
PyObject* pyvle_run_manager_matrix(vle::vpz::Vpz* file);
PyObject* pyvle_run_manager_thread(vle::vpz::Vpz* file, int th);
PyObject* pyvle_run_manager_thread_matrix(vle::vpz::Vpz* file, int th);
PyObject* pyvle_run_manager_cluster(vle::vpz::Vpz* file);
PyObject* pyvle_run_manager_cluster_matrix(vle::vpz::Vpz* file);

/* conditions */

PyObject* pyvle_condition_size(vle::vpz::Vpz* file);
PyObject* pyvle_condition_list(vle::vpz::Vpz* file);
PyObject* pyvle_condition_show(vle::vpz::Vpz* file,
			       std::string conditionname,
			       std::string portname);
void pyvle_condition_create(vle::vpz::Vpz* file,
                    const std::string name);
PyObject* pyvle_condition_port_list_size(vle::vpz::Vpz* file,
					 std::string conditionname);
PyObject* pyvle_condition_port_list(vle::vpz::Vpz* file,
				    std::string conditionname);
void pyvle_condition_port_clear(vle::vpz::Vpz* file,
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
void pyvle_condition_add_boolean(vle::vpz::Vpz* file,
				std::string conditionname,
				std::string portname,
				std::string value);
void pyvle_condition_add_value(vle::vpz::Vpz* file,
				 std::string conditionname,
				 std::string portname,
				 vle::value::Value* value);
void pyvle_condition_set_port_value(vle::vpz::Vpz* file,
			       std::string conditionname,
			       std::string portname,
			       vle::value::Value* value,
			       int i);
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
PyObject* pyvle_dynamic_get_model_list(vle::vpz::Vpz* file,
				std::string dynamicname);
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
PyObject* pyvle_list_view_entries(vle::vpz::Vpz* file);
PyObject* pyvle_get_output_plugin(vle::vpz::Vpz* file,
				std::string outputname);
PyObject* pyvle_observables_list(vle::vpz::Vpz* file);
PyObject* pyvle_outputs_list(vle::vpz::Vpz* file);
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
PyObject* pyvle_observable_port_attached_views(vle::vpz::Vpz* file,
				std::string obsname,
				std::string portname);
PyObject* pyvle_dynamic_observables_list(vle::vpz::Vpz* file,
				std::string name);
PyObject* pyvle_export(vle::vpz::Vpz* file,
		std::string location,
		std::string view,
		std::string type);
PyObject* pyvle_export_manager(vle::vpz::Vpz* file,
		std::string location,
		std::string view,
		std::string type);
PyObject* pyvle_nbreplicas(vle::vpz::Vpz* file);
PyObject* pyvle_combinations(vle::vpz::Vpz* file);
PyObject* pyvle_experiment_get_name(vle::vpz::Vpz* file);
PyObject* pyvle_trace_run_error(vle::vpz::Vpz* file);
PyObject* pyvle_get_installed_packages();
PyObject* pyvle_get_package_vpz_list(std::string name);
PyObject* pyvle_get_package_vpz_directory(std::string name);
PyObject* pyvle_get_package_data_directory(std::string name);
PyObject* pyvle_get_package_vpz(std::string name, std::string vpz);
void pyvle_set_package_mode(std::string name);
void pyvle_set_normal_mode();
void pyvle_set_output_plugin(vle::vpz::Vpz* file,
				  std::string outputname,
				  std::string location,
				  std::string format,
				  std::string plugin);
PyObject* pyvle_get_output_format(vle::vpz::Vpz* file,
                                  std::string outputname);
PyObject* pyvle_get_output_location(vle::vpz::Vpz* file,
                                    std::string outputname);
PyObject* pyvle_run_combination(vle::vpz::Vpz* file,
                                int comb);
void pyvle_set_nbreplicas(vle::vpz::Vpz* file,
                          int number);
PyObject* pyvle_get_seedreplicas(vle::vpz::Vpz* file);
void pyvle_set_seedreplicas(vle::vpz::Vpz* file,
                            long number);

vle::value::Value* pyvle_create_map();
vle::value::Value* pyvle_create_set();
vle::value::Value* pyvle_create_tuple(unsigned int size);
vle::value::Value* pyvle_create_table(unsigned int width, unsigned int height);
vle::value::Value* pyvle_create_matrix(unsigned int width, unsigned int height);



vle::value::Value* pyvle_int_to_value(long i);
vle::value::Value* pyvle_real_to_value(float i);
vle::value::Value* pyvle_string_to_value(std::string i);
vle::value::Value* pyvle_bool_to_value(bool i);
vle::value::Value* pyvle_str_to_xml(std::string i);

void pyvle_add_value_to_map(vle::value::Value* map, std::string key,
                            vle::value::Value* val);
void pyvle_add_value_to_set(vle::value::Value* set, vle::value::Value* val);
void pyvle_set_value_to_tuple(vle::value::Value* set, unsigned int i, double v);
void pyvle_set_value_to_table(vle::value::Value* set, unsigned int i,
        unsigned int j, double v);
void pyvle_set_value_to_matrix(vle::value::Value* set, unsigned int i,
        unsigned int j, vle::value::Value* v);

#endif
