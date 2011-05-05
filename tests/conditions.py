#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest as ut
import pyvle

class TestConditions(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("dummy.vpz")

    def testCreateConditionStillExists(self):
        name = "test"
        self.assertRaises(Exception, self.exp.createCondition, name)

    def testCreateCondition(self):
        name = "CONFIG"
        self.exp.createCondition(name)
        conds = self.exp.listConditions()
        self.assertEqual(True, (name in conds))

    def testCreateConditionWithPort(self):
        name = "CONFIG"
        port = {'test': 3}
        self.exp.createCondition(name, **port)
        self.assertEqual(port['test'],
                        self.exp.getConditionPortValue(name, 'test', 0))

def main():
    ut.main()

if __name__ == '__main__':
    main()

