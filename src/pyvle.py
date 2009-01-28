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

# condition methods
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
