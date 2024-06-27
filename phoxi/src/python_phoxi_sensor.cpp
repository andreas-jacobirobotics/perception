#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <phoxi_sensor.h>


namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal

PYBIND11_MODULE(phoxi, m) {
    m.doc() = "Photoneo PhoXi driver";

    py::class_<PhoxiSensor>(m, "PhoxiSensor")
        .def(py::init<py::str, py::str, py::str>(), "frame"_a, "device_name"_a, "size"_a)
        .def("start", &PhoxiSensor::start)
        .def("stop", &PhoxiSensor::stop)
        .def("frames", &PhoxiSensor::frames)
        .def("get_depth_map", &PhoxiSensor::get_depth_map)
        .def("get_texture", &PhoxiSensor::get_texture)
        // .def("get_point_cloud", &PhoxiSensor::get_point_cloud)
        // .def("get_normal_map", &PhoxiSensor::get_normal_map)
        .def_readonly("fx", &PhoxiSensor::fx)
        .def_readonly("fy", &PhoxiSensor::fy)
        .def_readonly("cx", &PhoxiSensor::cx)
        .def_readonly("cy", &PhoxiSensor::cy)
        .def_readonly("frame", &PhoxiSensor::frame)
        .def_readonly("ir_frame", &PhoxiSensor::frame);
}
