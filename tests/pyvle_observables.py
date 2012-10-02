#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest as ut
import pyvle
import sys

class TestObservables(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("test_simulation.vpz","test_port")

    def testSimError(self):
        self.exp.setOutputPlugin("view2","","local","dummy","vle.output")
    	self.exp.setOutputPlugin("view","","local","storage","vle.output")
    	res = self.exp.run()
    	self.assertTrue(isinstance(res, dict))
    	self.assertEqual(len(res), 1)
    	self.assertTrue(isinstance(res['view'], dict))
    	self.assertEqual(len(res["view"]['Top model:Perturb.obsPort']), 21)
    	self.assertEqual(res["view"]['Top model:Perturb.obsPort'][2],0)
    	self.assertEqual(res["view"]['Top model:Perturb.obsPort'][3],1.5)

if __name__ == '__main__':
    ut.main()

