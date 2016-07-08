#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest as ut
import pyvle
import sys

class TestConditions(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("test_error.vpz", "test_port")

    def testCreateConditionStillExists(self):
        name = "cond"
        self.assertEqual(len(self.exp.listConditions()),2)#cond and simulation_engine
        self.exp.createCondition(name)
        self.assertEqual(len(self.exp.listConditions()),2)#no cond added

    def testCreateCondition(self):
        name = "condCreate"
        self.exp.createCondition(name)
        conds = self.exp.listConditions()
        self.assertEqual(True, (name in conds))

    def testCreateConditionWithPort(self):
        name = "CONFIG"
        port = {'test': 3}
        self.exp.createCondition(name, **port)
        self.assertEqual(port['test'],
                        self.exp.getConditionPortValue(name, 'test', 0))
                        
    def testBooleanCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cBOOL", 0)
        self.assertTrue(isinstance(x, bool))
        self.assertEqual(x, True)
        self.exp.setConditionPortValue("cond", "cBOOL", False, 0)
        x = self.exp.getConditionPortValue("cond", "cBOOL", 0)
        self.assertTrue(isinstance(x, bool))
        self.assertEqual(x, False)
        
    def testIntegerCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cINTEGER", 0)
        self.assertTrue(isinstance(x, int))
        self.assertEqual(x, 3)
        self.exp.setConditionPortValue("cond", "cINTEGER", 2, 0)
        x = self.exp.getConditionPortValue("cond", "cINTEGER", 0)
        self.assertTrue(isinstance(x, int))
        self.assertEqual(x, 2)

    def testDoubleCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cDOUBLE", 0)
        self.assertTrue(isinstance(x, float))
        self.assertEqual(x, 4.8)
        self.exp.setConditionPortValue("cond", "cDOUBLE", 8.5, 0)
        x = self.exp.getConditionPortValue("cond", "cDOUBLE", 0)
        self.assertTrue(isinstance(x, float))
        self.assertEqual(x, 8.5)
        
    def testStringCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cSTRING", 0)
        self.assertTrue(isinstance(x, str))
        self.assertEqual(x, 'hello')
        self.exp.setConditionPortValue("cond", "cSTRING", 'hello2', 0)
        x = self.exp.getConditionPortValue("cond", "cSTRING", 0)
        self.assertTrue(isinstance(x, str))
        self.assertEqual(x, 'hello2')
        
    def testXMLCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cXML", 0)
        self.assertTrue(isinstance(x, pyvle.VleXML))
        self.assertEqual(x.val, '<mytag>hello</mytag>')
        self.exp.setConditionPortValue("cond", "cXML", pyvle.VleXML('<mytag>hello2</mytag>'), 0)
        x = self.exp.getConditionPortValue("cond", "cXML", 0)
        self.assertTrue(isinstance(x,pyvle.VleXML))
        self.assertEqual(x.val, '<mytag>hello2</mytag>')
        self.exp.clearConditionPort("cond", "cXML")
        self.exp.addXMLCondition("cond","cXML", pyvle.VleXML('<mytag>hello3</mytag>'))
        x = self.exp.getConditionPortValue("cond", "cXML", 0)
        self.assertTrue(isinstance(x,pyvle.VleXML))
        self.assertEqual(x.val, '<mytag>hello3</mytag>')
        
    def testSetCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cSET", 0)
        self.assertTrue(isinstance(x, list))
        self.assertEqual(len(x), 2)
        self.assertTrue(isinstance(x[1], pyvle.VleTuple))
        self.assertTrue(isinstance(x[1].val[0], float))
        self.assertEqual(x[1].val[0],1.9)
        x[1].val[0] = 2.8
        self.exp.setConditionPortValue("cond", "cSET",x, 0)
        x = self.exp.getConditionPortValue("cond", "cSET", 0)
        self.assertAlmostEqual(x[1].val[0],2.8)
        
    def testMapCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cMAP", 0)
        self.assertTrue(isinstance(x, dict))
        self.assertEqual(len(x), 2)
        self.assertAlmostEqual(x['key1'], 2.7)
        x['key2'] = 1.3
        self.exp.setConditionPortValue("cond", "cMAP",x, 0)
        x = self.exp.getConditionPortValue("cond", "cMAP", 0)
        self.assertAlmostEqual(x['key2'], 1.3)    
    
    def testTupleCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cTUPLE", 0)
        self.assertTrue(isinstance(x, pyvle.VleTuple))
        self.assertEqual(len(x.val), 3)
        self.assertAlmostEqual(x.val[1], 0)
        x.val[1] = 1.3
        self.exp.setConditionPortValue("cond", "cTUPLE",x, 0)
        x = self.exp.getConditionPortValue("cond", "cTUPLE", 0)
        self.assertAlmostEqual(x.val[1], 1.3)

    def testTableCondition(self):
    	x = self.exp.getConditionPortValue("cond", "cTABLE", 0)
        self.assertTrue(isinstance(x, pyvle.VleTable))
        self.assertEqual(len(x.val), 3)
        self.assertTrue(isinstance(x.val[1], list))
        self.assertEqual(len(x.val[1]), 2)
        self.assertTrue(isinstance(x.val[1][1], float))
        self.assertAlmostEqual(x.val[1][1], 2)
        x.val[1][1] = 1.3
        self.exp.setConditionPortValue("cond", "cTABLE",x, 0)
        x = self.exp.getConditionPortValue("cond", "cTABLE", 0)
        self.assertAlmostEqual(x.val[1][1], 1.3)

    def testMatrixCondition(self):
        typeMatrix = self.exp.getConditionValueType("cond", "cMATRIX", 0)
        self.assertEqual(typeMatrix, "matrix")
        x = self.exp.getConditionPortValue("cond", "cMATRIX", 0)
        self.assertTrue(isinstance(x, pyvle.VleMatrix))
        self.assertEqual(len(x.val), 2)
        self.assertTrue(isinstance(x.val[1], list))
        self.assertEqual(len(x.val[1]), 4)
        self.assertTrue(isinstance(x.val[1][3], list))
        self.assertEqual(len(x.val[1][3]), 2)
        self.assertAlmostEqual(x.val[1][3][0], 0.8)
        x.val[1][3][0] = 1.3
        self.exp.setConditionPortValue("cond", "cMATRIX",x, 0)
        x = self.exp.getConditionPortValue("cond", "cMATRIX", 0)
        self.assertAlmostEqual(x.val[1][3][0], 1.3)

    def testChekOnAddFunctions(self):
        x = self.exp.delConditionValue("cond", "cVALUE", 0)
	with self.assertRaises(ValueError) as cm:
            self.exp.addTupleCondition("cond", "cVALUE", True)
	the_exception = cm.exception
	self.assertEqual(the_exception.message,
		u"Can't convert type <type 'bool'> to VleTuple")

class TestConditions2(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("dummy.vpz", "test_port")

    def testCreateConditionStillExists(self):
        name = "test"
        self.assertEqual(len(self.exp.listConditions()),2)#test and simulation_engine
        self.exp.createCondition(name)
        self.assertEqual(len(self.exp.listConditions()),2)#no cond added

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


if __name__ == '__main__':
    ut.main()

