#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::array_t<float> impute_image(py::array_t<uint16_t> image, py::array_t<float> calibrationImage)
{
    auto clbImage = calibrationImage.mutable_unchecked<2>(); // x must have ndim = 2; can be non-writeable
    auto img = image.mutable_unchecked<2>();                 // x must have ndim = 2; can be non-writeable

    // py::array_t<float> myArray({clbImage.shape(0), clbImage.shape(1)});
    // auto r2 = myArray.mutable_unchecked<2>();

    // // initialize array with 0's
    // for (py::ssize_t i = 0; i < clbImage.shape(0); i++)
    //     for (py::ssize_t j = 0; j < clbImage.shape(1); j++)
    //     {
    //         r2(i, j) = 0;
    //     }

    for (py::ssize_t i = 0; i < clbImage.shape(0); i++)
        for (py::ssize_t j = 0; j < clbImage.shape(1); j++)
        {
            if ((i == 0) || (j == 0) || i == clbImage.shape(0) - 1 || j == clbImage.shape(1) - 1)
            {
            }
            else
            {
                float calibrationValue = clbImage(i, j);

                const uint16_t neighbouringIndices[][2] = {
                    {i + 1, j},
                    {i + 1, j + 1},
                    {i + 1, j - 1},
                    {i - 1, j},
                    {i - 1, j + 1},
                    {i - 1, j - 1},
                    {i, j + 1},
                    {i, j - 1}};
                if (calibrationValue < 0)
                {
                    uint16_t total = 0;
                    int count = 0;
                    uint16_t mean = 0;
                    for (auto &indices : neighbouringIndices)
                    {
                        float neighbourgingCalibValue = clbImage(indices[0], indices[1]);

                        // check which pixels are 0 around the query pixel in the calibration image
                        if (neighbourgingCalibValue >= 0)
                        {
                            total += img(indices[0], indices[1]);
                            count += 1;
                        }

                        // and use those to compute the average in the actual image}
                        mean = total / count;

                        // impute the pixel in the actual image
                        img(i, j) = mean;

                        // update the calibration image from negative to 0 for that index
                        clbImage(i, j) = 0;
                    }
                }
            }
        }
    return image;
}

PYBIND11_MODULE(pyoniip, m)
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
