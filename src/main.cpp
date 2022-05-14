#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::array_t<double> impute_image(py::array_t<uint16_t> image, py::array_t<float> calibrationImage)
{
    py::buffer_info buf1 = image.request();
    py::buffer_info buf2 = calibrationImage.request();

    if (buf1.size != buf2.size)
    {
        throw std::runtime_error("Input shapes must match");
    }

    /*  allocate the buffer */
    py::array_t<double> result = py::array_t<double>(buf1.size);

    py::buffer_info buf3 = result.request();

    double *ptr1 = (double *)buf1.ptr,
           *ptr2 = (double *)buf2.ptr,
           *ptr3 = (double *)buf3.ptr;
    int X = buf1.shape[0];
    int Y = buf1.shape[1];

    for (size_t idx = 0; idx < X; idx++)
    {
        for (size_t idy = 0; idy < Y; idy++)
        {
            ptr3[idx * Y + idy] = ptr1[idx * Y + idy] + ptr2[idx * Y + idy];
        }
    }

    // reshape array to match input shape
    result.resize({X, Y});

    return result;
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
