#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Loads and runs all the tests.
"""

import unittest as ut
import shutil
import tempfile
import sys
import os
import pyvle

def main():

    # add here the names of the unittest.TestCase
    # derived classes to use
    modules = (
                'pyvle_experiment.TestOpen',
                'pyvle_experiment.TestExperiment2',
                'pyvle_conditions.TestConditions2',
                'pyvle_simerror.TestSimError',
                'pyvle_observables.TestObservables',
                'pyvle_experiment.TestExperiment',
                'pyvle_conditions.TestConditions'
              )

    suite = ut.TestLoader().loadTestsFromNames(modules)
    ret = not ut.TextTestRunner(verbosity=1).run(suite).wasSuccessful()
    sys.exit(ret)

if __name__ == '__main__':
    sourcevlehome = sys.argv[1]
    tmpvlehome = "/tmp/vlehome"
    #tmpvlehome = tempfile.mkdtemp()+"/vlehome"
    shutil.rmtree(tmpvlehome, ignore_errors=True)
    os.chdir(sourcevlehome)
    shutil.copytree("vlehome", tmpvlehome)
    os.environ['VLE_HOME']=tmpvlehome
    pyvle.__compileTestPackages()
    main()
