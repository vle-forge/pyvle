#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# File:   pyvle.py
# Author: The VLE Development Team.
# Brief:  Python wrapper of VLE
#
# VLE Environment - the multimodeling and simulation environment
# This file is a part of the VLE environment (http://vle-project.org)
# Copyright (C) 2003 - 2012 The VLE Development Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

##
## pyvalue to vlevalue
## fonction (if specified types vleTUPLE, vleTABLE, vleXML):
##  bool                                    -> BOOLEAN
##  int                                     -> INTEGER
##  float                                   -> DOUBLE
##  str                                     -> STRING
##  VleXML (embeds str)                     -> XMLTYPE
##  list                                    -> SET
##  dict (with str keys)                    -> MAP
##  VleTuple (embeds list of float)         -> TUPLE
##  VleTable (embeds list of list of float) -> TABLE
##  VleMatrix (embeds list of list)         -> MATRIX
##
##
##  BOOLEAN  -> bool
##  INTEGER  -> int
##  DOUBLE   -> float
##  STRING   -> str
##  XMLTYPE  -> VleXML
##  SET      -> list
##  MAP      -> dict
##  TUPLE    -> VleTuple
##  TABLE    -> VleTable
##  MATRIX   -> VleMatrix
##

import libpyvle

class VleValue:
    _val_type = None
    
    def __init__(self, x):
        if self._val_type is None:
            raise NotImplementedError
        if isinstance(x, self._val_type):
			self.val = x
        else:
            raise ValueError(u'Can\'t embed type %s in %s' % (type(x)),
                                                    self.__class__.__name__)

    def __repr__(self):
        return "<%s(%r)>" % (self.__class__.__name__, self.val)

class VleXML(VleValue):
    _val_type = str

class VleTuple(VleValue):
	_val_type = list

class VleTable(VleValue):
	_val_type = list

class VleMatrix(VleValue):
	_val_type = list


class Vle:
    
    def __init__(self, file_, package = ""):
        if isinstance(file_, basestring):
            # assume file_ is a filename
            if package == "":
                self.vpz = libpyvle.open(file_)
            else:
                self.vpz = libpyvle.open_pkg(package, file_)
            self.filename = file_
        else:
            # assume file_ is a file object
            if package == "":
                self.vpz = libpyvle.from_buffer(file_.read())
            else:
                self.vpz = libpyvle.from_buffer_pkg(package, file_.read())
            self.filename = file_.name if hasattr(file_, "name") else None

    def save(self, file_):
        if isinstance(file_, basestring):
            # assume file_ is a filename
            libpyvle.save(self.vpz, file_)
        else:
            # assume file_ is a file object
            file_.write(libpyvle.save_buffer(self.vpz))


# name of experiments
    def setName(self, name):
        libpyvle.experiment_set_name(self.vpz, name)

# begin of experiments
    def setBegin(self, date):
        libpyvle.experiment_set_begin(self.vpz, date)

    def getBegin(self):
        return libpyvle.experiment_get_begin(self.vpz)

# duration of experiments
    def setDuration(self, value):
        libpyvle.experiment_set_duration(self.vpz, value)

    def getDuration(self):
        return libpyvle.experiment_get_duration(self.vpz)

# seed of experiments
    def setSeed(self, value):
        libpyvle.experiment_set_seed(self.vpz, value)

    def getSeed(self):
        return libpyvle.experiment_get_seed(self.vpz)

# replicas
    def setLinearCombination(self, seed, replicas):
        return libpyvle.experiment_set_linear_combination(
            self.vpz, seed, replicas)

    def setTotalCombination(self, seed, replicas):
        return libpyvle.experiment_set_total_combination(
            self.vpz, seed, replicas)

#################
## pyvle specific
    def getReplicaNumber(self):
        return libpyvle.nbreplicas(self.vpz)

    def setReplicaNumber(self, nb):
        libpyvle.set_nb_replicas(self.vpz, nb)

    def getReplicaSeed(self):
        return libpyvle.get_seed_replicas(self.vpz)

    def setReplicaSeed(self, seed):
        libpyvle.set_seed_replicas(self.vpz, seed)

    def getCombinations(self):
        return libpyvle.combinations(self.vpz)
##
#################

# execution of experiments
    def run(self):
        return libpyvle.run(self.vpz)

    def runMatrix(self):
        return libpyvle.run_matrix(self.vpz)

    def runManager(self):
        return libpyvle.run_manager(self.vpz)

    def runManagerMatrix(self):
        return libpyvle.run_manager_matrix(self.vpz)

    def runManagerThread(self, th):
        return libpyvle.run_manager_thread(self.vpz, th)

    def runManagerThreadMatrix(self, th):
        return libpyvle.run_manager_thread_matrix(self.vpz, th)

    def runManagerCluster(self):
        return libpyvle.run_manager_cluster(self.vpz)

    def runManagerClusterMatrix(self):
        return libpyvle.run_manager_cluster_matrix(self.vpz)

# conditions
    def listConditions(self):
        return libpyvle.condition_list(self.vpz)

    def listConditionPorts(self, name):
        return libpyvle.condition_port_list(self.vpz, name)

    def clearConditionPort(self, name, port):
        libpyvle.condition_port_clear(self.vpz, name, port);

    def getConditionPortValues(self, name, port):
        return libpyvle.condition_show(self.vpz, name, port)

    def getConditionPortValue(self, name, port, i):
        return libpyvle.condition_get_value(self.vpz, name, port, i)

# conditions add
    def addRealCondition(self, name, port, v):
        if isinstance(v,float):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to float' % type(v))

    def addIntegerCondition(self, name, port, v):
        if isinstance(v,int):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to int' % type(v))

    def addStringCondition(self, name, port, v):
        if isinstance(v,str):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to str' % type(v))

    def addBooleanCondition(self, name, port, v):
        if isinstance(v,bool):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to bool' % type(v))

    def addMapCondition(self, name, port, v):
        if isinstance(v,dict):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to dict' % type(v))

    def addSetCondition(self, name, port, v):
        if isinstance(v,list):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to list' % type(v))

    def addMatrixCondition(self, name, port, v):
        if isinstance(v,VleMatrix):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to VleMatrix' % type(v))

    def addTableCondition(self, name, port, v):
        if isinstance(v,VleTable):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to VleTable' % type(v))

    def addTupleCondition(self, name, port, v):
        if isinstance(v,VleTuple):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to VleTuple' % type(v))

    def addXMLCondition(self, name, port, v):
        if isinstance(v,VleXML):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))
        else:
            raise ValueError(u'Can\'t convert type %s to VleXML' % type(v))

    def addValueCondition(self, name, port, v):
            libpyvle.condition_add_value(self.vpz, name, port, to_value(v))

#################
## pyvle specific
    def createCondition(self, name, **ports):
        # ports is an optional list a keyworgs args :
        # portname = value, ...
        libpyvle.condition_create(self.vpz, name)
        for portname, val in ports.iteritems():
            libpyvle.condition_add_value(self.vpz, name,
                                        portname, to_value(val))

    def setConditionValue(self, name, port, value, type, i):
        libpyvle.condition_set_value(self.vpz, name, port, value, type, i)

    def setConditionPortValue(self, name, port, value, i):
        libpyvle.condition_set_port_value(self.vpz, name, port, to_value(value), i)

    def getConditionSetValue(self, name, port):
        return libpyvle.condition_get_setvalue(self.vpz, name, port)

    def getConditionValueType(self, name, port, i):
        return libpyvle.condition_get_value_type(self.vpz, name, port, i)

    def delConditionValue(self, name, port, i):
        libpyvle.condition_delete_value(self.vpz, name, port, i)

    def listAtomicModelConditions(self, name):
        return libpyvle.atomic_model_conditions_list(self.vpz, name)

    def listDynamicConditions(self, name):
        return libpyvle.dynamic_conditions_list(self.vpz, name)
##
#################

# views
    def listViews(self):
        return libpyvle.views_list(self.vpz)

    def getViewName(self, name):
        return libpyvle.view_get_name(self.vpz, name)

    def getViewType(self, name):
        return libpyvle.view_get_type(self.vpz, name)

    def getViewTimeStep(self, name):
        return libpyvle.view_get_timestep(self.vpz, name)

    def getViewOutput(self, name):
        return libpyvle.view_get_output(self.vpz, name)

    def getViewData(self, name):
        return libpyvle.view_get_data(self.vpz, name)

    def setViewName(self, oldname, newname):
        libpyvle.view_set_name(self.vpz, oldname, newname)

    def setViewType(self, name, type):
        libpyvle.view_set_type(self.vpz, name, type)

    def setViewTimeStep(self, name, timestep):
        libpyvle.view_set_timestep(self.vpz, name, timestep)

    def setViewData(self, name, data):
        libpyvle.view_set_data(self.vpz, name, data)

    def addEventView(self, name, output):
        libpyvle.views_add_eventview(self.vpz, name, output)

    def addTimedView(self, name, output, time):
        libpyvle.views_add_timedview(self.vpz, name, output, time)

    def addFinishView(self, name, output):
        libpyvle.views_add_finishview(self.vpz, name, output)

    def getOutputPlugin(self, output):
        return libpyvle.output_get_plugin(self.vpz, output)

    def setOutputPlugin(self, output, location, format, plugin):
        libpyvle.output_set_plugin(self.vpz, output, location, format, plugin)

    def getOutputFormat(self, output):
        return libpyvle.output_get_format(self.vpz, output)

    def getOutputLocation(self, output):
        return libpyvle.output_get_location(self.vpz, output)

    def listOutputs(self):
        return libpyvle.outputs_list(self.vpz)



# observables
    def listObservables(self):
        return libpyvle.observables_list(self.vpz)

    def addObservable(self, name):
        libpyvle.observable_add(self.vpz, name)

    def delObservable(self, name):
        libpyvle.observable_del(self.vpz, name)

    def existObservable(self, name):
        return libpyvle.observable_exists(self.vpz, name)

    def clearObservables(self):
        libpyvle.observables_clear(self.vpz)

    def isObservablesEmpty(self):
        return libpyvle.observables_empty(self.vpz)

    def getObservableName(self, name):
        return libpyvle.observable_get_name(self.vpz, name)

    def listObservablePorts(self, name):
        return libpyvle.observable_ports_list(self.vpz, name)

    def addObservablePort(self, obsname, portname):
        libpyvle.observable_add_port(self.vpz, obsname, portname)

    def delObservablePort(self, obsname, portname):
        libpyvle.observable_del_port(self.vpz, obsname, portname)

    def hasObservableView(self, obsname, viewname):
        return libpyvle.observable_has_view(self.vpz, obsname, viewname)

    def getObservablePortName(self, obsname, viewname):
        return libpyvle.observable_get_port_name(self.vpz, obsname, viewname)

    def isPermanentObservable(self, obsname):
        return libpyvle.observable_is_permanent(self.vpz, obsname)

    def setPermanentObservable(self, obsname, ispermanent):
        libpyvle.observable_set_permanent(self.vpz, obsname, ispermanent)

    def getObservablePortAttachedViews(self, obsname, portname):
    	return libpyvle.observable_port_attached_views(self.vpz, obsname,
                                                       portname)

    def listDynamicObservables(self, name):
        return libpyvle.dynamic_observables_list(self.vpz, name)

    def listViewsEntries(self):
        return libpyvle.list_view_entries(self.vpz)

# dynamics
    def listDynamics(self):
        return libpyvle.dynamics_list(self.vpz)

    def getDynamicName(self, name):
        return libpyvle.dynamic_get_name(self.vpz, name)

    def getDynamicModel(self, name):
        return libpyvle.dynamic_get_model(self.vpz, name)

    def getDynamicLibrary(self, name):
        return libpyvle.dynamic_get_library(self.vpz, name)

    def getDynamicLanguage(self, name):
        return libpyvle.dynamic_get_language(self.vpz, name)

    def setDynamicModel(self, name, model):
        libpyvle.dynamic_set_model(self.vpz, name, model)

    def setDynamicLibrary(self, name, library):
        libpyvle.dynamic_set_library(self.vpz, name, library)

    def setDynamicLanguage(self, name, language):
        libpyvle.dynamic_set_language(self.vpz, name, language)

    def listDynamicModels(self, name):
        return libpyvle.dynamic_get_model_list(self.vpz, name)

# export
    def export(self, location, view, type):
        return libpyvle.export(self.vpz, location, view, type)

    def exportManager(self, location, view, type):
        return libpyvle.export_manager(self.vpz, location, view, type)

# other
    def getExperimentName(self):
        return libpyvle.experiment_get_name(self.vpz)

    def traceRunError(self):
        return libpyvle.trace_run_error(self.vpz)

    def setPackageMode(self, name):
        libpyvle.set_package_mode(name)

    def setNormalMode(self):
        libpyvle.set_normal_mode()

    def runCombination(self, comb):
        return libpyvle.run_combination(self.vpz, comb)

class VlePackage:
    def __init__(self, name):
        self.name = name

    @classmethod
    def getInstalledPackages(cls):
        return libpyvle.get_installed_packages()

    def getVpzList(self):
        return libpyvle.get_package_vpz_list(self.name)

    def getVpzDirectory(self):
        return libpyvle.get_package_vpz_directory(self.name)

    def getDataDirectory(self):
        return libpyvle.get_package_data_directory(self.name)

    def getOutputDirectory(self):
        return libpyvle.get_package_output_directory(self.name)

    def getVpz(self, vpz):
        return libpyvle.get_package_vpz(self.name, vpz)

def to_value(x):
	if isinstance(x, bool):
		val = libpyvle.bool_to_value(x)
	elif isinstance(x, int):
		val = libpyvle.int_to_value(x)
	elif isinstance(x, float):
		val = libpyvle.real_to_value(x)
	elif isinstance(x, str):
		val = libpyvle.string_to_value(x)
	elif isinstance(x, dict):
		val = libpyvle.create_map()
		for k,v in x.iteritems():
			libpyvle.add_value_to_map(val, k, to_value(v))
	elif isinstance(x, list):
		val = libpyvle.create_set()
		for v in x:
			libpyvle.add_value_to_set(val, to_value(v))
	elif isinstance(x, VleTuple):
		if isinstance(x.val,list):
			val = libpyvle.create_tuple(len(x.val))
			i = 0
			for v in x.val:
				if isinstance(v,float):
					libpyvle.set_value_to_tuple(val, i, v)
					i = i+1
				else:
					raise ValueError(u'Can\'t convert type %s to float' % type(v))
		else:
			raise ValueError(u'Can\'t convert type %s to list' % type(x.val))
	elif isinstance(x, VleTable):
		val = None
		i = 0
		for v in x.val:
			if isinstance(v,list):
				j = 0
				for v1 in v:
					if isinstance(v1,float):
						if (val == None):
							val = libpyvle.create_table(len(v),len(x.val))
						libpyvle.set_value_to_table(val, j,i,v1)
						j = j+1
					else:
						raise ValueError(u'Can\'t convert type %s to float' % type(v1))
				i = i+1
			else:
				raise ValueError(u'Can\'t convert type %s to list' % type(v))
	elif isinstance(x, VleMatrix):
		val = None
		i = 0
		for v in x.val:
			if isinstance(v,list):
				j = 0
				for v1 in v:
					if (val == None):
						val = libpyvle.create_matrix(len(v),len(x.val))
					libpyvle.set_value_to_matrix(val,j,i,to_value(v1))
					j = j+1
				i = i+1
			else:
				raise ValueError(u'Can\'t convert type %s to list' % type(v))
	elif isinstance(x, VleXML):
		if isinstance(x.val,str):
			val = libpyvle.str_to_xml(x.val)
		else:
			raise ValueError(u'Can\'t convert type %s to str' % type(x.val))
	elif isinstance(x, libpyvle.Value):
		val = x
	else:
		raise ValueError(u'Can\'t convert type %s in vle::value::Value' %
						 type(x))
	return val
