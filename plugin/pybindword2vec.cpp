#include "pybindword2vec.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
PYBIND11_MODULE(word2vec, m)
{
  namespace py = pybind11;
  py::class_<CWord2Vec, std::shared_ptr<CWord2Vec>>(m, "Word2Vec")
   //py::class_<PyProfiler,nvinfer1::IProfiler>(m, "PyProfiler")
      .def(py::init<>())
        // The destroy_plugin function does not override the base class, so we must bind it explicitly.
      .def("myadd", &CWord2Vec::myadd)
      .def("loadWord2Vec", &CWord2Vec::loadWord2Vec)
      .def("loadDoc2VecModel", &CWord2Vec::loadDoc2VecModel)
      .def("saveDoc2VecModel", &CWord2Vec::saveDoc2VecModel)
      .def("doc2vector", &CWord2Vec::convertDoc2Vec)
      //.def("doc2vecmap", &CWord2Vec::getMapData)
      .def("doc2vectorSimilarity", &CWord2Vec::Doc2VecSimilarity);
      //.def(py::init<PyProfiler::reportLayerTime_t>());
}

