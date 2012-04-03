/*
 * @file src/wrapper.cpp
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


#include <pyvle.hpp>
#include <boost/python.hpp>
#include <boost/noncopyable.hpp>
#include <vle/value.hpp>

BOOST_PYTHON_MODULE(libpyvle)
{
    namespace bp = boost::python;

    bp::class_ < vle::vpz::Vpz > ("Vpz");
    bp::class_ < vle::value::Value , boost::noncopyable > ("Value", bp::no_init);

    bp::def("create_map",
            pyvle_create_map, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("create_set",
            pyvle_create_set, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("int_to_value",
            pyvle_int_to_value, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("real_to_value", pyvle_real_to_value,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("string_to_value", pyvle_string_to_value,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("bool_to_value", pyvle_bool_to_value,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("add_value_to_map", pyvle_add_value_to_map);
    bp::def("add_value_to_set", pyvle_add_value_to_set);

    bp::def("create_table",
            pyvle_create_table, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("create_matrix",
            pyvle_create_matrix, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("create_tuple",
            pyvle_create_tuple, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("create_table",
            pyvle_create_table, bp::return_value_policy < bp::manage_new_object > ());
    bp::def("set_value_to_tuple", pyvle_set_value_to_tuple);
    bp::def("set_value_to_table", pyvle_set_value_to_table);
    bp::def("set_value_to_matrix", pyvle_set_value_to_matrix);
    bp::def("str_to_xml", pyvle_str_to_xml,
            bp::return_value_policy < bp::manage_new_object > ());

    bp::def("open", pyvle_open,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("open_pkg", pyvle_open_pkg,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("from_buffer", pyvle_from_buffer,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("from_buffer_pkg", pyvle_from_buffer_pkg,
            bp::return_value_policy < bp::manage_new_object > ());
    bp::def("save", pyvle_save);
    bp::def("save_buffer", pyvle_save_buffer);
    bp::def("delete", pyvle_delete);
    bp::def("experiment_set_name", pyvle_experiment_set_name);
    bp::def("experiment_set_begin", pyvle_experiment_set_begin);
    bp::def("experiment_get_begin", pyvle_experiment_get_begin);
    bp::def("experiment_set_duration", pyvle_experiment_set_duration);
    bp::def("experiment_get_duration", pyvle_experiment_get_duration);
    bp::def("experiment_set_seed", pyvle_experiment_set_seed);
    bp::def("experiment_get_seed", pyvle_experiment_get_seed);
    bp::def("experiment_set_linear_combination",
            pyvle_experiment_set_linear_combination);
    bp::def("experiment_set_total_combination",
            pyvle_experiment_set_total_combination);
    bp::def("run", pyvle_run);
    bp::def("run_matrix", pyvle_run_matrix);
    bp::def("run_manager", pyvle_run_manager);
    bp::def("run_manager_matrix", pyvle_run_manager_matrix);
    bp::def("run_manager_thread", pyvle_run_manager_thread);
    bp::def("run_manager_thread_matrix", pyvle_run_manager_thread_matrix);
    bp::def("run_manager_cluster", pyvle_run_manager_cluster);
    bp::def("run_manager_cluster_matrix", pyvle_run_manager_cluster_matrix);
    bp::def("condition_list", pyvle_condition_list);
    bp::def("condition_show", pyvle_condition_show);
    bp::def("condition_create", pyvle_condition_create);
    bp::def("condition_port_list", pyvle_condition_port_list);
    bp::def("condition_port_clear", pyvle_condition_port_clear);
    bp::def("condition_add_real", pyvle_condition_add_real);
    bp::def("condition_add_integer", pyvle_condition_add_integer);
    bp::def("condition_add_string", pyvle_condition_add_string);
    bp::def("condition_add_boolean", pyvle_condition_add_boolean);
    bp::def("condition_add_value", pyvle_condition_add_value);
    bp::def("condition_set_port_value", pyvle_condition_set_port_value);
    bp::def("condition_set_value", pyvle_condition_set_value);
    bp::def("condition_get_setvalue", pyvle_condition_get_setvalue);
    bp::def("condition_get_value", pyvle_condition_get_value);
    bp::def("condition_get_value_type", pyvle_condition_get_value_type);
    bp::def("condition_delete_value", pyvle_condition_delete_value);
    bp::def("atomic_model_conditions_list", pyvle_atomic_model_conditions_list);
    bp::def("dynamic_conditions_list", pyvle_dynamic_conditions_list);
    bp::def("dynamics_list", pyvle_dynamics_list);
    bp::def("dynamic_get_name", pyvle_dynamic_get_name);
    bp::def("dynamic_get_model", pyvle_dynamic_get_model);
    bp::def("dynamic_get_library", pyvle_dynamic_get_library);
    bp::def("dynamic_get_language", pyvle_dynamic_get_language);
    bp::def("dynamic_set_model", pyvle_dynamic_set_model);
    bp::def("dynamic_set_library", pyvle_dynamic_set_library);
    bp::def("dynamic_set_language", pyvle_dynamic_set_language);
    bp::def("dynamic_get_model_list", pyvle_dynamic_get_model_list);
    bp::def("views_list", pyvle_views_list);
    bp::def("view_get_name", pyvle_view_get_name);
    bp::def("view_get_type", pyvle_view_get_type);
    bp::def("view_get_timestep", pyvle_view_get_timestep);
    bp::def("view_get_output", pyvle_view_get_output);
    bp::def("view_get_data", pyvle_view_get_data);
    bp::def("view_set_name", pyvle_view_set_name);
    bp::def("view_set_type", pyvle_view_set_type);
    bp::def("view_set_timestep", pyvle_view_set_timestep);
    bp::def("view_set_data", pyvle_view_set_data);
    bp::def("views_add_eventview", pyvle_views_add_eventview);
    bp::def("views_add_timedview", pyvle_views_add_timedview);
    bp::def("views_add_finishview", pyvle_views_add_finishview);
    bp::def("list_view_entries", pyvle_list_view_entries);
    bp::def("output_get_plugin", pyvle_get_output_plugin);
    bp::def("observables_list", pyvle_observables_list);
    bp::def("observable_add", pyvle_observable_add);
    bp::def("observable_del", pyvle_observable_del);
    bp::def("observable_exists", pyvle_observable_exists);
    bp::def("observables_clear", pyvle_observables_clear);
    bp::def("observables_empty", pyvle_observables_empty);
    bp::def("observable_get_name", pyvle_observable_get_name);
    bp::def("observable_ports_list", pyvle_observable_ports_list);
    bp::def("observable_add_port", pyvle_observable_add_port);
    bp::def("observable_del_port", pyvle_observable_del_port);
    bp::def("observable_has_view", pyvle_observable_has_view);
    bp::def("observable_get_port_name", pyvle_observable_get_port_name);
    bp::def("observable_is_permanent", pyvle_observable_is_permanent);
    bp::def("observable_set_permanent", pyvle_observable_set_permanent);
    bp::def("observable_port_attached_views", pyvle_observable_port_attached_views);
    bp::def("dynamic_observables_list", pyvle_dynamic_observables_list);
    bp::def("export", pyvle_export);
    bp::def("export_manager", pyvle_export_manager);
    bp::def("nbreplicas", pyvle_nbreplicas);
    bp::def("combinations", pyvle_combinations);
    bp::def("experiment_get_name", pyvle_experiment_get_name);
    bp::def("trace_run_error", pyvle_trace_run_error);
    bp::def("get_installed_packages", pyvle_get_installed_packages);
    bp::def("get_package_vpz_list", pyvle_get_package_vpz_list);
    bp::def("get_package_vpz_directory", pyvle_get_package_vpz_directory);
    bp::def("get_package_data_directory", pyvle_get_package_data_directory);
    bp::def("get_package_vpz", pyvle_get_package_vpz);
    bp::def("set_package_mode", pyvle_set_package_mode);
    bp::def("set_normal_mode", pyvle_set_normal_mode);
    bp::def("output_set_plugin", pyvle_set_output_plugin);
    bp::def("output_get_format", pyvle_get_output_format);
    bp::def("output_get_location", pyvle_get_output_location);
    bp::def("run_combination", pyvle_run_combination);
    bp::def("set_nb_replicas", pyvle_set_nbreplicas);
    bp::def("get_seed_replicas", pyvle_get_seedreplicas);
    bp::def("set_seed_replicas", pyvle_set_seedreplicas);
    bp::def("outputs_list", pyvle_outputs_list);

}
