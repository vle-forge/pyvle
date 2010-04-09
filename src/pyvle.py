import libpyvle

class Vle:
    def __init__(self, filename, package = ""):
        if package == "":
            self.vpz = libpyvle.open(filename)
        else:
            self.vpz = libpyvle.open_pkg(package, filename)
        self.filename = filename;

    def save(self, filename):
        libpyvle.save(self.vpz, filename)

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

    def addRealCondition(self, name, port, value):
        libpyvle.condition_add_real(self.vpz, name, port, value)

    def addIntegerCondition(self, name, port, value):
        libpyvle.condition_add_integer(self.vpz, name, port, value)

    def addStringCondition(self, name, port, value):
        libpyvle.condition_add_string(self.vpz, name, port, value)

    def addBooleanCondition(self, name, port, value):
        libpyvle.condition_add_boolean(self.vpz, name, port, value)

#################
## pyvle specific
    def setConditionValue(self, name, port, value, type, i):
        libpyvle.condition_set_value(self.vpz, name, port, value, type, i)

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
        return libpyvle.trace_run_error(self.filename)

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

    def getVpz(self, vpz):
        return libpyvle.get_package_vpz(self.name, vpz)

