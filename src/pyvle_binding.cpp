//#include "vle_binding.hpp"
#include <pybind11/pybind11.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarraytypes.h>
#include <Python.h>

#include <vle/value/Table.hpp>
#include <vle/value/XML.hpp>

#include "pyvle.hpp"


namespace py = pybind11;

namespace pybind11 { namespace detail {
template <> struct type_caster<VleValue> {
public:
    /**
     * This macro establishes the name 'inty' in
     * function signatures and declares a local variable
     * 'value' of type inty
     */
    PYBIND11_TYPE_CASTER(VleValue, _("VleValue"));

    /**
     * Conversion part 1 (Python->C++): convert a PyObject into a inty
     * instance or return false upon failure. The second argument
     * indicates whether implicit conversions should be applied.
     */
    bool load(handle src, bool) {
        /* Extract PyObject from handle */
        try {
            if (not value.set(src.ptr())) return false;
        } catch (...) {
            return false;
        }
        return !PyErr_Occurred();
    }

    /**
     * Conversion part 2 (C++ -> Python): convert an inty instance into
     * a Python object. The second and third arguments are used to
     * indicate the return value policy and parent object (for
     * ``return_value_policy::reference_internal``) and are generally
     * ignored by implicit casters.
     */
    static handle cast(VleValue src, return_value_policy /* policy */,
            handle /* parent */) {
        return src.get_pyobject();
    }
};
}} // namespace pybind11::detail


PYBIND11_MODULE(pyvle, m) 
{
    //building functions
    py::class_<pyvle::Vle>(m, "Vle")
      .def(py::init<const std::string&>())
      .def(py::init<const std::string&, const std::string&>());
    //compile test_port
    m.def("__compile_test_port", &pyvle::__compile_test_port);
    //static functions
    m.def("packages_list", &pyvle::packages_list);
    m.def("package_content", &pyvle::package_content);
    //basic functions
    m.def("save", &pyvle::save);
    m.def("set_log_level", &pyvle::set_log_level);
    m.def("get_atomic_models", &pyvle::get_atomic_models);
    m.def("get_conditions", &pyvle::get_conditions);
    m.def("add_condition", &pyvle::add_condition);
    m.def("del_condition", &pyvle::del_condition);
    m.def("get_attached_conditions", &pyvle::get_attached_conditions);
    m.def("attach_condition", &pyvle::attach_condition);
    m.def("detach_condition", &pyvle::detach_condition);
    m.def("get_condition_ports", &pyvle::get_condition_ports);
    m.def("add_condition_port", &pyvle::add_condition_port);
    m.def("del_condition_port", &pyvle::del_condition_port);
    m.def("get_condition_port_value", &pyvle::get_condition_port_value);
    m.def("set_condition_port_value", &pyvle::set_condition_port_value);
    m.def("get_observables", &pyvle::get_observables);
    m.def("get_observable_ports", &pyvle::get_observable_ports);
    m.def("add_observable_port", &pyvle::add_observable_port);
    m.def("del_observable_port", &pyvle::del_observable_port);
    m.def("attach_view", &pyvle::attach_view);
    m.def("detach_view", &pyvle::detach_view);
    m.def("get_attached_views", &pyvle::get_attached_views);
    m.def("get_views", &pyvle::get_views);
    m.def("add_view", &pyvle::add_view);
    m.def("del_view", &pyvle::del_view);
    m.def("get_view_config", &pyvle::get_view_config);
    m.def("set_view_config", &pyvle::set_view_config);
    m.def("set_view_plugin", &pyvle::set_view_plugin);
    m.def("get_view_plugin", &pyvle::get_view_plugin);
    m.def("available_outputs", &pyvle::available_outputs);
    m.def("run", &pyvle::run);
    //manager functions
    m.def("manager_clear", &pyvle::manager_clear);
    m.def("manager_get_config", &pyvle::manager_get_config);
    m.def("manager_set_config", &pyvle::manager_set_config);
    //plan functions
    m.def("plan_clear", &pyvle::plan_clear);
    m.def("plan_get", &pyvle::plan_get);
    m.def("plan_define", &pyvle::plan_define);
    m.def("plan_input", &pyvle::plan_input);
    m.def("plan_propagate", &pyvle::plan_propagate);
    m.def("plan_replicate", &pyvle::plan_replicate);
    m.def("plan_output", &pyvle::plan_output);
    m.def("plan_run", &pyvle::plan_run);
    m.def("plan_embedded", &pyvle::plan_embedded);
    //experiment functions
    m.def("experiment_run", &pyvle::experiment_run);

}
