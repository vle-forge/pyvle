import libpyvle

class Vle:
    def __init__(self, filename):
        self.vpz = libpyvle.open(filename)

    def save(self):
        libpyvle.save(self.vpz)

# duration of experiments
    def duration(self, value):
        libpyvle.set_duration(self.vpz, value)

    def duration(self):
        return libpyvle.get_duration(self.vpz)

# seed of experiments
    def seed(self, value):
        libpyvle.set_seed(self.vpz, value)

    def seed(self):
        return libpyvle.get_seed(self.vpz)

# execution of experiments
    def run(self):
        return libpyvle.run(self.vpz)

    def run_matrix(self):
        return libpyvle.run_matrix(self.vpz)

# conditions
    def conditions(self):
        return libpyvle.condition_list(self.vpz)

    def conditionPorts(self, name):
        return libpyvle.condition_port_list(self.vpz, name)

    def clearConditions(self):
        libpyvle.condition_clear(self.vpz)

    def showCondition(self, name, port):
        return libpyvle.condition_show(self.vpz, name, port)

    def addRealCondition(self, name, port, value):
        libpyvle.condition_add_real(self.vpz, name, port, value)

    def addIntegerCondition(self, name, port, value):
        libpyvle.condition_add_integer(self.vpz, name, port, value)

    def addStringCondition(self, name, port, value):
        libpyvle.condition_add_string(self.vpz, name, port, value)

# views

# observables

# dynamics
    def dynamics(self):
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