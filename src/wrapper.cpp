/**
 * @file        wrapper.cpp
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

#include "pyvle.hpp"
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(libpyvle)
{
    class_<vle::vpz::Vpz>("Vpz");

    def("open", pyvle_open, return_value_policy<manage_new_object>());
    def("delete", pyvle_delete);
    def("run", pyvle_run);
    def("run_matrix", pyvle_run_matrix);
    def("manager", pyvle_manager);
    def("manager_matrix", pyvle_manager_matrix);
    def("manager_thread", pyvle_manager_thread);
    def("manager_thread_matrix", pyvle_manager_thread_matrix);
    def("manager_cluster", pyvle_manager_cluster);
    def("manager_cluster_matrix", pyvle_manager_cluster_matrix);
    def("condition_list", pyvle_condition_list);
    def("condition_show", pyvle_condition_show);
    def("condition_port_list", pyvle_condition_port_list);
    def("condition_clear", pyvle_condition_clear);
    def("condition_add_real", pyvle_condition_add_real);
    def("condition_add_integer", pyvle_condition_add_integer);
    def("condition_add_string", pyvle_condition_add_string);
    def("condition_add_boolean", pyvle_condition_add_boolean);
    def("condition_set_value",pyvle_condition_set_value);
    def("condition_get_setvalue",pyvle_condition_get_setvalue);
    def("condition_get_value",pyvle_condition_get_value);
    def("condition_get_value_type",pyvle_condition_get_value_type);
    def("condition_delete_value",pyvle_condition_delete_value);
    def("atomic_model_conditions_list",pyvle_atomic_model_conditions_list);
    def("dynamic_conditions_list",pyvle_dynamic_conditions_list);
    def("experiment_set_duration", pyvle_experiment_set_duration);
    def("experiment_get_duration", pyvle_experiment_get_duration);
    def("experiment_set_seed", pyvle_experiment_set_seed);
    def("experiment_get_seed", pyvle_experiment_get_seed);
    def("save", pyvle_save);
    def("dynamics_list", pyvle_dynamics_list);
    def("dynamic_get_name", pyvle_dynamic_get_name);
    def("dynamic_get_model", pyvle_dynamic_get_model);
    def("dynamic_get_library", pyvle_dynamic_get_library);
    def("dynamic_get_language", pyvle_dynamic_get_language);
    def("dynamic_set_model", pyvle_dynamic_set_model);
    def("dynamic_set_library", pyvle_dynamic_set_library);
    def("dynamic_set_language", pyvle_dynamic_set_language);
    def("dynamic_get_model_list", pyvle_dynamic_get_model_list);
    def("views_list", pyvle_views_list);
    def("view_get_name",pyvle_view_get_name);
    def("view_get_type",pyvle_view_get_type);
    def("view_get_timestep",pyvle_view_get_timestep);
    def("view_get_output",pyvle_view_get_output);
    def("view_get_data",pyvle_view_get_data);
    def("view_set_name",pyvle_view_set_name);
    def("view_set_type",pyvle_view_set_type);
    def("view_set_timestep",pyvle_view_set_timestep);
    def("view_set_data",pyvle_view_set_data);
    def("views_add_eventview",pyvle_views_add_eventview);
    def("views_add_timedview",pyvle_views_add_timedview);
    def("views_add_finishview",pyvle_views_add_finishview);
    def("output_get_plugin",pyvle_get_output_plugin);
    def("observables_list",pyvle_observables_list);
    def("observable_add",pyvle_observable_add);
    def("observable_del",pyvle_observable_del);
    def("observable_exists",pyvle_observable_exists);
    def("observables_clear",pyvle_observables_clear);
    def("observables_empty",pyvle_observables_empty);
    def("observable_get_name",pyvle_observable_get_name);
    def("observable_ports_list",pyvle_observable_ports_list);
    def("observable_add_port",pyvle_observable_add_port);
    def("observable_del_port",pyvle_observable_del_port);
    def("observable_has_view",pyvle_observable_has_view);
    def("observable_get_port_name",pyvle_observable_get_port_name);
    def("observable_is_permanent",pyvle_observable_is_permanent);
    def("observable_set_permanent",pyvle_observable_set_permanent);
    def("observable_port_attached_views", pyvle_observable_port_attached_views);
    def("dynamic_observables_list",pyvle_dynamic_observables_list);
    def("export",pyvle_export);
    def("export_manager",pyvle_export_manager);
    def("nbreplicas",pyvle_nbreplicas);
    def("combinations",pyvle_combinations);
    def("experiment_get_name",pyvle_experiment_get_name);
    def("trace_run_error",pyvle_trace_run_error);
}
