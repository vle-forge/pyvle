#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Loads and runs all the tests.
"""

import unittest as ut
import pyvle
import sys

def main():

    # add here the names of the unittest.TestCase
    # derived classes to use
    modules = (
                'experiment.TestOpen',
                'experiment.TestExperiment',
                'conditions.TestConditions',
                )

    suite = ut.TestLoader().loadTestsFromNames(modules)
    ut.TextTestRunner(verbosity=1).run(suite)

if __name__ == '__main__':
    if len(sys.argv) >= 2:
        main()
