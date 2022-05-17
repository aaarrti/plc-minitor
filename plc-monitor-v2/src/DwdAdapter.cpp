//
// Created by Artem Sereda on 22.10.21.
//

#include "DwdAdapter.h"
#include <python3.8/Python.h>

#include "spdlog/spdlog.h"

std::string DwdAdapter::getWeather(const std::string& id, const std::string& start, const std::string& end) {

  setenv("PYTHONPATH", "/tmp/service/src/", 1);
  Py_Initialize();
  PyImport_ImportModule("dwd_adapter");
  PyObject* myModuleString = PyUnicode_FromString("dwd_adapter");
  PyObject* myModule = PyImport_Import(myModuleString);
  PyObject* myFunction = PyObject_GetAttrString(myModule,"getWeather");
  PyObject* myResult = PyObject_CallFunction(myFunction, "sss", id.c_str(), start.c_str(), end.c_str());
  std::string result = PyUnicode_AsUTF8(myResult);
  Py_Finalize();
  spdlog::info("dwd_adapter returned: {}", result);
  return result;

}
