#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest as ut
import pyvle
import libpyvle
import sys
import os
import tempfile as tmp


dummy_vpz = """<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE vle_project PUBLIC "-//VLE TEAM//DTD Strict//EN" "http://www.vle-project.org/vle-1.0.0.dtd">
<vle_project version="1.0" date="Fri, 13 Apr 2012" author="Gauthier Quesnel">
<structures>
<model name="Top model" type="coupled" x="0" y="0" width="484"  >
<submodels>
</submodels>
<connections>
</connections>
</model>
</structures>
<experiment name="test" duration="1.000000000000000" begin="0.000000000000000" combination="linear" seed="123456789" >
<conditions>
<condition name="test" >
</condition>
</conditions>
<views>
</views>
</experiment>
</vle_project>
"""

class TestOpen(ut.TestCase):
    
    def testOpenFromFilename(self):
        # existing file
        exp = pyvle.Vle("dummy.vpz", "test_port")
        self.assertIsInstance(exp.vpz, libpyvle.Vpz)
        # not existing file
        nfilename = "not_existing_file"
        exp = pyvle.Vle(nfilename)
        self.assertIsNone(exp.vpz)

    def testOpenFromFileObject(self):
        dummyPath = os.path.normpath(os.environ["VLE_HOME"]+"/pkgs-1.1/test_port/exp/dummy.vpz")
        with open(dummyPath, 'r') as f:
            exp = pyvle.Vle(f)
            self.assertIsInstance(exp.vpz, libpyvle.Vpz)
    
    def testOpenFromStringBuffer(self):
        from cStringIO import StringIO
        s = StringIO(dummy_vpz)
        exp = pyvle.Vle(s)
        self.assertIsInstance(exp.vpz, libpyvle.Vpz)
        # bad vpz
        s = StringIO("not a valid vpz")
        exp = pyvle.Vle(s)
        self.assertIsNone(exp.vpz)


class TestExperiment(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("dummy.vpz", "test_port")
    
    def testSaveInFilename(self):
        with tmp.NamedTemporaryFile() as dumped:
            self.exp.save(dumped.name)
            exp_test = pyvle.Vle(dumped.name)
            self.assertIsInstance(exp_test.vpz, libpyvle.Vpz)
    
    def testSaveInFileObject(self):
        with tmp.NamedTemporaryFile(delete=False) as dumped:
            self.exp.save(dumped)
        with open(dumped.name, 'r') as f:
            exp_test = pyvle.Vle(f)
            self.assertIsInstance(exp_test.vpz, libpyvle.Vpz)
        os.remove(dumped.name)
    
    def testSaveInStringBuffer(self):
        from cStringIO import StringIO
        s = StringIO()
        self.exp.save(s)
        s.seek(0)
        exp_test = pyvle.Vle(s)
        self.assertIsInstance(exp_test.vpz, libpyvle.Vpz)

    def testGetBegin(self):
        self.assertEqual(0, self.exp.getBegin())

    def testSetBeginBadInput(self):
        self.assertRaises(Exception, self.exp.setBegin, 'a')

    def testSetBegin(self):
        self.exp.setBegin(1)
        self.assertEqual(1, self.exp.getBegin())

class TestExperiment2(ut.TestCase):

    def setUp(self):
        self.exp = pyvle.Vle("test_simulation.vpz","test_port")

    def testGetBegin(self):
        self.assertEqual(0, self.exp.getBegin())

    def testSetBeginBadInput(self):
        self.assertRaises(Exception, self.exp.setBegin, 'a')

    def testSetBegin(self):
        self.exp.setBegin(1)
        self.assertEqual(1, self.exp.getBegin())

if __name__ == '__main__':
    ut.main()

