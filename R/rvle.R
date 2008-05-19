#
# @file rvle.R
# @author The VLE Development Team
#

#
# VLE Environment - the multimodeling and simulation environment
# This file is a part of the VLE environment (http://vle.univ-littoral.fr)
# Copyright (C) 2003 - 2008 The VLE Development Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#



.First.lib <- function(lib, pkg)
{
    library.dynam("rvle", pkg, lib)
}

rvle.open <- function(file)
{
    .Call("open", as.character(file), PACKAGE="rvle")
}

rvle.run <- function(x)
{
    is.rvle(x)

    .Call("run", x, PACKAGE="rvle")
}

rvle.run_matrix <- function(x)
{
    is.rvle(x)

    .Call("run_matrix", x, PACKAGE="rvle")
}

rvle.run_manager <- function(x)
{
    is.rvle(x)

    .Call("run_manager", x, PACKAGE="rvle")
}

rvle.run_manager_matrix <- function(x)
{
    is.rvle(x)

    .Call("run_manager_matrix", x, PACKAGE="rvle")
}

rvle.run_manager_thread <- function(x, th)
{
    is.rvle(x)

    .Call("run_manager_thread", x, as.integer(th), PACKAGE="rvle")
}

rvle.run_manager_thread_matrix <- function(x, th)
{
    is.rvle(x)

    .Call("run_manager_thread_matrix", x, as.integer(th), PACKAGE="rvle")
}

rvle.run_manager_cluster <- function(x)
{
    is.rvle(x)

    .Call("run_manager_cluster", x, PACKAGE="rvle")
}

rvle.run_manager_cluster_matrix <- function(x)
{
    is.rvle(x)

    .Call("run_manager_cluster_matrix", x, PACKAGE="rvle")
}

is.rvle <- function(x)
{
    inherits(x, "rvle")
}

rvle.condition_list <- function(x)
{
    is.rvle(x)

    .Call("condition_list", x, PACKAGE="rvle")
}

rvle.condition_port_list <- function(x, condition)
{
    is.rvle(x)
    as.character(condition)

    .Call("condition_port_list", x, condition, PACKAGE="rvle")
}

rvle.condition_size <- function(x)
{
    is.rvle(x)

    .Call("condition_size", x, PACKAGE="rvle")
}

rvle.condition_port_list_size <- function(x, condition)
{
    is.rvle(x)
    as.character(condition)

    .Call("condition_port_list_size", x, condition, PACKAGE="rvle")
}

rvle.condition_set_real <- function(x, condition, port, value)
{
    is.rvle(x)
    as.character(condition)
    as.character(port)

    .Call("condition_set_real", x, condition, port, as.real(value),
            PACKAGE="rvle")
}

rvle.condition_set_integer <- function(x, condition, port, value)
{
    is.rvle(x)
    as.character(condition)
    as.character(port)

    .Call("condition_set_integer", x, condition, port, as.integer(value),
            PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.experiment_set_duration <- function(x, value)
{
    is.rvle(x)

    .Call("experiment_set_duration", x, as.real(value), PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.experiment_get_duration <- function(x)
{
    is.rvle(x)

    .Call("experiment_get_duration", x, PACKAGE="rvle")
}

rvle.save <- function(x, file)
{
    is.rvle(x)
    as.character(file)

    .Call("save", x, file, PACKAGE="rvle")

    return (invisible(NULL))
}

# vim:tw=80:ts=8:sw=4:sts=4
