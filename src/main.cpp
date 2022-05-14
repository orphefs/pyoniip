#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::array_t<float> impute_image(py::array_t<uint16_t> image, py::array_t<float> calibrationImage)
{
    auto r = calibrationImage.unchecked<2>(); // x must have ndim = 3; can be non-writeable

    py::array_t<float> myArray({r.shape(0), r.shape(1)});
    auto r2 = myArray.mutable_unchecked<2>();

    for (py::ssize_t i = 0; i < r.shape(0); i++)
        for (py::ssize_t j = 0; j < r.shape(1); j++)
        {

            r2(i, j) = r(i, j);

            const uint16_t neighbouring_indices[][2] = {
                {i + 1, j},
                {i + 1, j + 1},
                {i + 1, j - 1},
                {i - 1, j},
                {i - 1, j + 1},
                {i - 1, j - 1},
                {i, j + 1},
                {i, j - 1}};
        }

    return myArray;
}

PYBIND11_MODULE(oniip, m)
{
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: cmake_example
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

    m.def("impute_image", &impute_image, R"pbdoc(
        Add two arrays
        Some other explanation about the add function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
