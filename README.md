# Python port of VLE

A Python binding for the VFL.

See AUTHORS and COPYRIGHT for the list of contributors.

## Requirements

* python (>= 2.7 && < 3)
* vle (>= 2.0)
* boost (>= 1.47)
* cmake (>= 3.0)
* make (>= 1.8)
* c++ compiler (gcc >= 4.9, clang >= 3.1, intel icc (>= 11.0)

## Getting the code

The source tree is currently hosted on Github and Sourceforge. To view the
repository online: https://github.com/vle-forge/pyvle The URL to clone it:

    git clone git://github.com/vle-forge/pyvle.git

Once you have met requirements, compiling and installing is simple.

Installation in the distribution directory. If you wan to install pyvle into
the classical python directory (/usr/lib/python-2.x/), just follow the
commands:

    cd pyvle
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    make
    make install

Installation in `$HOME` directory. If you want to install pyvle into your
`$HOME` (or any other directory) directory, you need to add an environment
variable `$PYTHONPATH` into your shell. For instance for bash, add the
following line. Update the python2.7 with the version of the python you use:

    PYTHONPATH=$HOME/usr/lib/python2.7:$HOME/usr/lib/python2.7/lib-dynload

And follow the commands:

    cd pyvle
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=$HOME/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    make
    make install

## License

This software in GPLv3 or later. See the file COPYING. Some files are under a
different license. Check the headers for the copyright info.

## Usage

    $ python
    Python 3.6.9 (default, Apr 18 2020, 01:56:04)
    [GCC 8.4.0] on linux
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import pyvle
    >>> dir(pyvle)
    >>> f = pyvle.Vle("my_vpz","my_package")
    >>> print(f)
    <pyvle.Vle object at 0x7fd025169b20>
    >>> pyvle.get_conditions(f)
    ['cond', 'cond_A', 'cond_B', 'cond_C', 'cond_D', 'cond_sum' ]
    >>> pyvle.get_condition_ports(f, 'cond_A')
    ['name', 'value']
    >>> pyvle.get_condition_port_value(f, 'cond_A', 'value')
    0.0
    >>> pyvle.run(f)
    
