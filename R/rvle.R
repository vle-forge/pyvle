/** 
 * @file rvle.R
 * @brief 
 * @author The vle Development Team
 * @date 2007-11-14
 */

/*
 Copyright (C) 2007 - The vle Development Team
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

rvle.new <- function(file)
{
        .Call("new", as.character(file), PACKAGE="rvle")
}

rvle.run <- function(x)
{
        is.rvle(x)
        .Call("run", x, PACKAGE="rvle")
}

rvle.close <- function(x)
{
        is.rvle(x)
        .Call("close", x, PACKAGE="rvle")
}

is.rvle <- function(x)
{
        inherits(x, "rvle")
}
