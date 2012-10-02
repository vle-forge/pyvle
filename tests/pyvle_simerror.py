#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest as ut
import pyvle
import sys

class TestSimError(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("test_error.vpz","test_port")

    def testSimError(self):
    	r = self.exp.run()
        self.assertEqual(r,
        	'/!\\ vle error reported:  error at initialisation\n')

if __name__ == '__main__':
    ut.main()

