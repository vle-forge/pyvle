#ifndef PYVLE_CONVERT
#define PYVLE_CONVERT

//#include "VleBinding.hpp"
#include <pybind11/pybind11.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarraytypes.h>
#include <Python.h>

#include <vle/value/Boolean.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/Matrix.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Map.hpp>
#include <vle/value/String.hpp>
#include <vle/value/Tuple.hpp>
#include <vle/value/Table.hpp>
#include <vle/value/XML.hpp>

#include <iostream>

//
// Python -> c++
//
//  bool                                    -> BOOLEAN
//  int                                     -> INTEGER
//  float                                   -> DOUBLE
//  str                                     -> STRING
//  VleXML (embeds str)                     ->
//  list    []                              -> SET
//  set   {v1, v2}                          -> SET
//  dict (with str keys)   {k : v, k2: v2}  -> MAP
//  VleTuple (embeds list of float)         ->
//  np.array (dim 1)                        -> (not TUPLE)
//  VleTable (embeds list of list of float) ->
//  VleMatrix (embeds list of list)         ->
//  np.array (dim 2)                        -> (not TABLE)
//
//  BOOLEAN  -> bool
//  INTEGER  -> int
//  DOUBLE   -> float
//  STRING   -> str
//  XMLTYPE  -> str (not VleXML)
//  SET      -> list
//  MAP      -> dict
//  TUPLE    -> list of float (not VleTuple)
//  TABLE    -> list of list of float (not VleTable
//  MATRIX   -> list of list (not VleMatrix)

namespace vv = vle::value;

class VleValue
{
private:
    PyObject* mpyobject;
    vv::Value* mvlevalue;
public:
    VleValue() : mpyobject(0), mvlevalue(0)
    {
    }

    VleValue(const VleValue& p) : mpyobject(p.mpyobject), mvlevalue(p.mvlevalue)
    {
    }
    ~VleValue()
    {
    }

    bool set(vv::Value* v)
    {
        assert(!mvlevalue);
        assert(!mpyobject);
        mvlevalue = v;
        mpyobject = VleValue::vleToPyObject(*v);
        return true;
    }
    bool set(PyObject* v)
    {
        assert(!mvlevalue);
        assert(!mpyobject);
        mvlevalue = VleValue::vleFromPyObject(v).release();
        mpyobject = v;
        if (not mvlevalue) return false;
        return true;
    }

    vv::Value* get_vle_value()
    {
        assert(mvlevalue);
        assert(mpyobject);
        vv::Value* tmp = mvlevalue;
        mvlevalue = 0;
        mpyobject = 0;
        return tmp;
    }

    PyObject* get_pyobject()
    {
        assert(mvlevalue);
        assert(mpyobject);
        PyObject* tmp = mpyobject;
        mvlevalue = 0;
        mpyobject = 0;
        return tmp;
    }


    // Python -> c++
    static std::unique_ptr<vv::Value>
    vleFromPyObject(PyObject* source)
    {
        std::string sourcetype(Py_TYPE(source)->tp_name);

        std::unique_ptr<vv::Value> val = nullptr;
        if (sourcetype == "int") {
            val = std::move(vv::Integer::create(PyLong_AsLong(source)));
        } else if (sourcetype == "bool") {
            val = std::move(vv::Boolean::create(PyObject_IsTrue(source)));
        } else if (sourcetype == "str") {
            PyObject* source_utf8 = PyUnicode_AsUTF8String(source);
            std::string stand_string = PyBytes_AsString(source_utf8);
            Py_DECREF(source_utf8);
            val = std::move(vv::String::create(stand_string));
        } else if (sourcetype == "float") {
            val = std::move(vv::Double::create(PyFloat_AsDouble(source)));
        } else if (sourcetype == "list") {
            unsigned int listsize = PyList_GET_SIZE(source);
            val = std::move(vv::Set::create(listsize));
            for (unsigned int i=0; i<listsize; i++) {
                val->toSet().set(i, vleFromPyObject(PyList_GetItem(source, i)));
            }
        } else if (sourcetype == "set") {
            unsigned int setsize = PySet_GET_SIZE(source);
            val = std::move(vv::Set::create(setsize));
            int i =0;
            while (PySet_GET_SIZE(source)) {
                PyObject* val_i = PySet_Pop(source);
                val->toSet().set(i, vleFromPyObject(val_i));
                Py_DECREF(val_i);
                i++;
            }
        } else if (sourcetype == "dict") {
            PyObject *key, *pyval_i;
            Py_ssize_t pos = 0;
            val = std::move(vv::Map::create());
            std::string key_type;
            while (PyDict_Next(source, &pos, &key, &pyval_i)) {
                key_type.assign(Py_TYPE(key)->tp_name);
                if (key_type != "str") {
                    val.reset(nullptr);
                    return val;
                }
                PyObject* key_utf8 = PyUnicode_AsUTF8String(key);
                std::string key_string = PyBytes_AsString(key_utf8);
                val->toMap().add(key_string, vleFromPyObject(pyval_i));
                Py_DECREF(key_utf8);
            }
        } else if (sourcetype == "numpy.ndarray") {
            int ndim = PyArray_NDIM((PyArrayObject*) source);
            std::cout << " NP ARRAY " << ndim << "\n";
        }
        return val;
    }

    // c++ -> Python
    static PyObject*
    vleToPyObject(const vle::value::Value& value)
    {
        PyObject* result;

        switch (value.getType()) {
        case vle::value::Value::BOOLEAN: {
            result = PyBool_FromLong(
                    vle::value::toBoolean(value));
            break;
        }
        case vle::value::Value::INTEGER: {
            result = PyLong_FromLong(
                    vle::value::toInteger(value));
            break;
        }
        case vle::value::Value::DOUBLE: {
            result = PyFloat_FromDouble(
                    vle::value::toDouble(value));
            break;
        }
        case vle::value::Value::STRING: {
            result = PyUnicode_FromString(
                    vle::value::toString(value).c_str());
            break;
        }
        case vle::value::Value::XMLTYPE: {
            result = PyUnicode_FromString(vle::value::toXml(value).c_str());
            break;
        }
        case vle::value::Value::SET: {
            result = PyList_New(0);
            for (vle::value::Set::const_iterator it = value.toSet().begin();
                    it != value.toSet().end(); ++it) {
                PyList_Append(result, vleToPyObject(**it));
            }
            break;
        }
        case vle::value::Value::MAP: {
            result = PyDict_New();
            for (vle::value::Map::const_iterator it = value.toMap().begin();
                    it != value.toMap().end(); ++it) {
                PyDict_SetItemString(result, it->first.c_str(),
                        vleToPyObject(*(it->second)));
            }
            break;
        }
        case vle::value::Value::TUPLE: {
            result = PyList_New(0);
            std::vector<double>::const_iterator itb =
                    value.toTuple().value().begin();
            std::vector<double>::const_iterator ite =
                    value.toTuple().value().end();
            for(;itb!=ite;itb++){
                PyList_Append(result, PyFloat_FromDouble(*itb));
            }
            break;
        }
        case vle::value::Value::TABLE: {
            PyObject* result = PyList_New(0);
            PyObject* r=0;
            const vle::value::Table& t = value.toTable();
            for(unsigned int i=0; i<t.height(); i++){
                r = PyList_New(0);
                for(unsigned int j=0; j<t.width(); j++){
                    PyList_Append(r,PyFloat_FromDouble(t.get(j,i)));
                }
                PyList_Append(result,r);
            }
            break;
        }
        case vle::value::Value::MATRIX: {
            result = PyList_New(0);
            PyObject* r=0;
            const vle::value::Matrix& t = value.toMatrix();
            for(unsigned int i=0; i<t.rows(); i++){
                r = PyList_New(0);
                for(unsigned int j=0; j<t.columns(); j++){
                    PyList_Append(r,vleToPyObject(*t.get(j,i)));
                }
                PyList_Append(result,r);
            }
            break;
        }
        default: {
            result = Py_None;
            break;
        }
        }
        return result;
    }
};

#endif
