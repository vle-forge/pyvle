/** 
 * @file rvle.h
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

#ifndef VLE_RPACKAGE_VLE_H
#define VLE_RPACKAGE_VLE_H

#ifdef __cplusplus
extern "C" {
#endif


/** 
 * @brief An hande to an obscure structure.
 */
typedef void* RVLE;


/** 
 * @brief Open the file vpz filename using vpz library.
 * @param filename The vpz file to laod with his path.
 * @return A RVLE object or NULL on error.
 */
RVLE rvle_open(const char* filename);

/** 
 * @brief Run a simulation using the RVLE object.
 * @param handle The reference to the Vpz file.
 */
void rvle_run(RVLE handle);

/** 
 * @brief Flush data.
 * @param handle The reference to the Vpz file.
 */
void rvle_close(RVLE handle);

/**
 * @brief Destruction of the RVLE object.
 * @param handle The reference to the Vpz file.
 */
void rvle_delete(RVLE handle);


#ifdef __cplusplus
}
#endif

#endif
