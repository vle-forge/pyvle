#
# Copyright (C) 2007 - The vle Development Team
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

.First.lib <- function(lib, pkg) {
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

rvle.close <- function(x)
{
        is.rvle(x)
        .Call("close", x, PACKAGE="rvle")
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

rvle.condition_size <- function(x)
{
        is.rvle(x)
        .Call("condition_size", x, PACKAGE="rvle")
}

rvle.condition_set_real <- function(x, condition, port, value)
{
    is.rvle(x)
    as.character(condition)
    as.character(port)
    is.real(value)

    .Call("condition_set_real", x, condition, port, value, PACKAGE="rvle")
}
