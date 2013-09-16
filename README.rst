====================================
Virtual Laboratory Environment 1.1.0
====================================

A Python binding for the VFL.

See AUTHORS and COPYRIGHT for the list of contributors.

Changes from PyVLE 1.1.0-beta2
------------------------------

- README: switch to rst
- fix cpack script

Requirements
------------

* python (>= 2.7 && < 3)
* vle (>= 1.1)
* boost (>= 1.41)
* cmake (>= 2.8.0)
* make (>= 1.8)
* c++ compiler (gcc >= 4.4, clang >= 3.1, intel icc (>= 11.0)

Getting the code
----------------

The source tree is currently hosted on Github and Sourceforge. To view the
repository online: https://github.com/vle-forge/pyvle The URL to clone it:

::

 git clone git://github.com/vle-forge/pyvle.git

Once you have met requirements, compiling and installing is simple:

* Installation in the distribution directory. If you wan to install pyvle into
  the classical python directory (/usr/lib/python-2.x/), just follow the
  commands:

::

  cd pyvle
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
  make
  make install

* Installation in $HOME directory. If you want to install pyvle into your $HOME
  (or any other directory) directory, you need to add an environment variable
  PYTHONPATH into your shell. For instance for bash, add the following line.
  Update the python2.7 with the version of the python you use:

::

  PYTHONPATH=$HOME/usr/lib/python2.7:$HOME/usr/lib/python2.7/lib-dynload

* And follow the commands:

::

  cd pyvle
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX=$HOME/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
  make
  make install

License
-------

This software in GPLv3 or later. See the file COPYING. Some files are under a
different license. Check the headers for the copyright info.

Usage
-----

::

    $ python
    Python 2.7.3 (default, Sep 26 2012, 21:51:14)
    [GCC 4.7.2] on linux2
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import pyvle
    >>> from pyvle import Vle
    >>> dir(pyvle.Vle)
    >>> f = Vle("my_vpz","my_package")
    >>> print(f)
    <pyvle.Vle instance at 0x7fe231515e60>
    >>> f.listConditions()
    ['cond', 'cond_A', 'cond_B', 'cond_C', 'cond_D', 'cond_sum' ]
    >>> f.listConditionPorts('cond_A')
    ['name', 'value']
    >>> f.getConditionPortValues('cond_A', 'value')
    0.0
    >>> f.clearConditionPort('cond_A', 'value')
    >>> f.addRealCondition('cond_A', 'value', 1)
    >>> f.getConditionPortVal
