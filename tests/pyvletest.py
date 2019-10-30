#!/usr/bin/python3

"""
Loads and runs all the tests.
"""

import unittest 
import shutil
import tempfile
import sys
import os
import pyvle



class TestConditions(unittest.TestCase):
    def setUp(self):
        self.exp = pyvle.Vle("test_vle.vpz", "test_port")
        
    
    def testAtomicModelsList(self):
        atoms = pyvle.get_atomic_models(self.exp)
        self.assertTrue(isinstance(atoms, list))
        self.assertEqual(len(atoms), 1)
        self.assertEqual(atoms[0], 'Top model,Perturb')
        print ( 'testAtomicModelsList ')
 
    def testCreateConditionStillExists(self):
        name = "cond"
        self.assertEqual(len(pyvle.get_conditions(self.exp)),3)
        pyvle.add_condition(self.exp, name)
        self.assertEqual(len(pyvle.get_conditions(self.exp)),3)
        print ( 'testCreateConditionStillExists ')
 
    def testCreateCondition(self):
        name = "condCreate"
        pyvle.add_condition(self.exp, name)
        conds = pyvle.get_conditions(self.exp)
        self.assertEqual(True, (name in conds))
                          
    def testBooleanCondition(self):
        x = pyvle.get_condition_port_value(self.exp, "test", "bool")
        self.assertTrue(isinstance(x, bool))
        self.assertEqual(x, False)
        pyvle.set_condition_port_value(self.exp, "test", "bool", True)
        x = pyvle.get_condition_port_value(self.exp, "test", "bool")
        self.assertTrue(isinstance(x, bool))
        self.assertEqual(x, True)
          
    def testIntegerCondition(self):
        x = pyvle.get_condition_port_value(self.exp, "test", "int")
        self.assertTrue(isinstance(x, int))
        self.assertEqual(x, 2)
        pyvle.set_condition_port_value(self.exp, "test", "int", 3)
        x = pyvle.get_condition_port_value(self.exp, "test", "int")
        self.assertTrue(isinstance(x, int))
        self.assertEqual(x, 3)
  
    def testDoubleCondition(self):
        x = pyvle.get_condition_port_value(self.exp, "test", "double")
        self.assertTrue(isinstance(x, float))
        self.assertEqual(x, 0.1)
        pyvle.set_condition_port_value(self.exp, "test", "double", 8.5)
        x = pyvle.get_condition_port_value(self.exp, "test", "double")
        self.assertTrue(isinstance(x, float))
        self.assertEqual(x, 8.5)
          
    def testStringCondition(self):
        x = pyvle.get_condition_port_value(self.exp, "test", "string")
        self.assertTrue(isinstance(x, str))
        self.assertEqual(x, 'dummy')
        pyvle.set_condition_port_value(self.exp, "test", "string", 'hello2')
        x = pyvle.get_condition_port_value(self.exp, "test", "string")
        self.assertTrue(isinstance(x, str))
        self.assertEqual(x, 'hello2')
          
    def testMapCondition(self):
        x = pyvle.get_condition_port_value(self.exp, "test", "map")
        self.assertTrue(isinstance(x, dict))
        self.assertEqual(len(x), 2)
        self.assertAlmostEqual(x['a'], 0.1)
        x['b'] = 0.3
        pyvle.set_condition_port_value(self.exp, "test", "map", x)
        x = pyvle.get_condition_port_value(self.exp, "test", "map")
        self.assertAlmostEqual(x['b'], 0.3)

    def testAttachedConditions(self):
        att = pyvle.get_attached_conditions(self.exp, "Top model,Perturb")
        self.assertEqual(len(att), 2)
        pyvle.attach_condition(self.exp, "Top model,Perturb", "test")
        att = pyvle.get_attached_conditions(self.exp, "Top model,Perturb")
        self.assertTrue(isinstance(att, list))
        self.assertEqual(len(att), 3)
 
    def testObservables(self):
        o = pyvle.get_observables(self.exp)
        self.assertTrue(isinstance(o, list))
        self.assertEqual(len(o), 1)
        self.assertEqual(o[0],'obs')
        op = pyvle.get_observable_ports(self.exp, 'obs')
        self.assertTrue(isinstance(op, list))
        self.assertEqual(len(op), 1)
        self.assertEqual(op[0],'obsPort')
        opatt = pyvle.get_attached_views(self.exp, 'obs', "obsPort")
        self.assertTrue(isinstance(opatt, list))
        self.assertEqual(len(opatt), 2)
        self.assertEqual(opatt[0],'view')
        self.assertEqual(opatt[1],'view2')
        
    def testRun(self):
        res = pyvle.run(self.exp)
        self.assertTrue(isinstance(res, dict))
        self.assertEqual(len(res), 2)
        self.assertTrue(isinstance(res["view"], list))
        self.assertTrue(isinstance(res["view"][0], list))#first line
        self.assertEqual(res["view"][0][1],'Top model:Perturb.obsPort')

if __name__ == '__main__':
    #environment VLE_HOME_TEST and VLE_TEST_DIR must be set
    #eg : VLE_HOME_TEST=/tmp/vlehome  VLE_TEST_DIR=.  ./pyvletest.py
    sourcevlehome = os.environ['VLE_TEST_DIR']
    print(sourcevlehome)
    tmpvlehome = os.environ['VLE_HOME_TEST']
    print(tmpvlehome)
    shutil.rmtree(tmpvlehome, ignore_errors=True)
    os.mkdir(tmpvlehome)
    os.chdir(sourcevlehome)
    os.environ['VLE_HOME']=tmpvlehome
    pyvle.__compile_test_port()
    unittest.main()

