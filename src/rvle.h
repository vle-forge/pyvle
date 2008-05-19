/**
 * @file rvle.h
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



#ifndef VLE_RPACKAGE_VLE_H
#define VLE_RPACKAGE_VLE_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief An hande to an obscure structure. A vpz::Vpz::Vpz class in fact.
 */
typedef void* rvle_t;

/**
 * @brief An handle to an obscure structure. A vle::manager::OutputMatrix in
 * fact.
 */
typedef void* rvle_output_t;

/**
 * @brief Open the file vpz filename using vpz library.
 * @param filename The vpz file to laod with his path.
 * @return A rvle_t object or NULL if error.
 */
rvle_t rvle_open(const char* filename);

/**
 * @brief Run a simulation using the rvle_t object.
 * @param handle The reference to the Vpz file.
 * @return A rvle_output_t object or NULL if error.
 */
rvle_output_t rvle_run(rvle_t handle);

/**
 * @brief Run an experimental frames using the rvle_t object.
 * @param handle The reference to the Vpz file.
 * @return A rvle_output_t object or NULL if error.
 */
rvle_output_t rvle_manager(rvle_t handle);

/**
 * @brief Run an experimental frames int thread using the rvle_t object.
 * @param handle The reference to the Vpz file.
 * @param th The number of thread.
 * @return A rvle_output_t object or NULL if error.
 */
rvle_output_t rvle_manager_thread(rvle_t handle, int th);

/**
 * @brief Run an experimental frames on cluster using the rvle_t object.
 * @param handle The reference to the Vpz file.
 * @return A rvle_output_t object or NULL if error.
 */
rvle_output_t rvle_manager_cluster(rvle_t handle);

/**
 * @brief Destruction of the rvle_t object.
 * @param handle The reference to the Vpz file.
 */
void rvle_delete(rvle_t handle);

/**
 * @brief Get the list of conditions lists.
 * @param handle The reference to the Vpz file.
 * @return The reference to a char**. Memory use malloc, don't forget to use
 * free function.
 */
char** rvle_condition_list(rvle_t handle);

/**
 * @brief Get the list of port in the specified condition list.
 * @param handle The reference to the Vpz file.
 * @param conditionname the name of the condition.
 * @return The reference to a char**. Memory use malloc, don't forget to use
 * free function.
 */
char** rvle_condition_port_list(rvle_t handle, const char* conditionname);

/**
 * @brief Get the number of conditions in conditions lists.
 * @param handle The reference to the Vpz file.
 * @return The number of conditions.
 */
int rvle_condition_size(rvle_t handle);

/**
 * @brief Get the number of portname for the specified condition.
 * @param handle The reference to the Vpz file.
 * @param conditionname The name of the condition to get port list.
 * @return The number of conditions, -1 on error.
 */
int rvle_condition_port_list_size(rvle_t handle, const char* conditionname);

/**
 * @brief Set the initial condition of the specified condition and portname.
 * @param handle The reference to the Vpz file.
 * @param conditionname The name of the condition.
 * @param portname The name of the condition's port.
 * @param value The value to push.
 * @return 0 if failed, -1 otherwise.
 */
int rvle_condition_set_real(rvle_t handle,
                            const char* conditionname,
                            const char* portname,
                            double value);

/**
 * @brief Set the initial condition of the specified condition and portname.
 * @param handle The reference to the Vpz file.
 * @param conditionname The name of the condition.
 * @param portname The name of the condition's port.
 * @param value The value to push.
 * @return 0 if failed, -1 otherwise.
 */
int rvle_condition_set_integer(rvle_t handle,
                               const char* conditionname,
                               const char* portname,
                               long value);

/**
 * @brief Set the duration of the experiment.
 * @param handle the reference to the Vpz file.
 * @param value the duration to set, must be greather than 0.
 * @return 0 if failed, -1 otherwise.
 */
int rvle_experiment_set_duration(rvle_t handle, double value);

/**
 * @brief Get the duration of the experiment.
 * @param handle The reference to the Vpz file.
 * @return the duration of the experiment.
 */
double rvle_experiment_get_duration(rvle_t handle);

/**
 * @brief Save the current file under the specified filename.
 * @param handle The reference to the Vpz file.
 * @param filename The filename where store file.
 */
int rvle_save(rvle_t handle, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
