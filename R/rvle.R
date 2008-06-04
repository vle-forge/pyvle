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
    stopifnot(is.character(file))

    x <- .Call("open", file, PACKAGE="rvle")
    stopifnot(!is.null(x))
    class(x) <- 'rvle'
    return(x)
}

rvle.run <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run", f, PACKAGE="rvle")
}

rvle.run_matrix <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run_matrix", f, PACKAGE="rvle")
}

rvle.run_manager <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run_manager", f, PACKAGE="rvle")
}

rvle.run_manager_matrix <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run_manager_matrix", f, PACKAGE="rvle")
}

rvle.run_manager_thread <- function(f, th)
{
    stopifnot(is.rvle(f))

    .Call("run_manager_thread", f, as.integer(th), PACKAGE="rvle")
}

rvle.run_manager_thread_matrix <- function(f, th)
{
    stopifnot(is.rvle(f))

    .Call("run_manager_thread_matrix", f, as.integer(th), PACKAGE="rvle")
}

rvle.run_manager_cluster <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run_manager_cluster", f, PACKAGE="rvle")
}

rvle.run_manager_cluster_matrix <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("run_manager_cluster_matrix", f, PACKAGE="rvle")
}

is.rvle <- function(f)
{
    inherits(f, "rvle")
}

rvle.condition_list <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("condition_list", f, PACKAGE="rvle")
}

rvle.condition_port_list <- function(f, condition)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))

    .Call("condition_port_list", f, condition, PACKAGE="rvle")
}

rvle.condition_size <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("condition_size", f, PACKAGE="rvle")
}

rvle.condition_port_list_size <- function(f, condition)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))

    .Call("condition_port_list_size", f, condition, PACKAGE="rvle")
}

rvle.condition_clear <- function(f, condition, port)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    .Call("condition_clear", f, condition, port, PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.condition_add_real <- function(f, condition, port, value)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    .Call("condition_add_real", f, condition, port, as.real(value),
            PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.condition_set_real <- function(f, condition, port, value)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    rvle.condition_clear(f, condition, port)
    rvle.condition_add_real(f, condition, port, value)

    return (invisible(NULL))
}

rvle.condition_add_integer <- function(f, condition, port, value)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    .Call("condition_add_integer", f, condition, port, as.integer(value),
            PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.condition_set_integer <- function(f, condition, port, value)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    rvle.condition_clear(f, condition, port)
    rvle.condition_add_integer(f, condition, port, value)

    return (invisible(NULL))
}

rvle.experiment_set_duration <- function(f, value)
{
    stopifnot(is.rvle(f))

    .Call("experiment_set_duration", f, as.real(value), PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.experiment_get_duration <- function(f)
{
    stopifnot(is.rvle(f))

    .Call("experiment_get_duration", f, PACKAGE="rvle")
}

rvle.save <- function(f, file)
{
    stopifnot(is.rvle(f))
    stopifnot(is.character(file))

    .Call("save", f, file, PACKAGE="rvle")

    return (invisible(NULL))
}

# vim:tw=80:ts=8:sw=4:sts=4
