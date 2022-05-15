#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::array_t<uint16_t> impute_image(const py::array_t<uint16_t> &image, const py::array_t<float> &calibrationImage)
{

    auto srcImg = image.unchecked<2>(); // x must have ndim = 2; can be non-writeable

    // init new array for destination
    py::array_t<uint16_t> result({srcImg.shape(0), srcImg.shape(1)});
    auto destImg = result.mutable_unchecked<2>();

    // init copy of calibrationImage
    py::array_t<float> calibrationImage2({calibrationImage.shape(0), calibrationImage.shape(1)});
    auto clbImage = calibrationImage2.mutable_unchecked<2>(); // x must have ndim = 2; can be non-writeable
    auto srcClbImage = calibrationImage.unchecked<2>();       // x must have ndim = 2; can be non-writeable

    // copy calibrationImage to calibrationImage2
    for (py::ssize_t i = 0; i < srcClbImage.shape(0); i++)
        for (py::ssize_t j = 0; j < srcClbImage.shape(1); j++)
        {
            clbImage(i, j) = srcClbImage(i, j);
        }

    // copy srcImg to destImg
    for (py::ssize_t i = 0; i < srcImg.shape(0); i++)
        for (py::ssize_t j = 0; j < srcImg.shape(1); j++)
        {
            destImg(i, j) = srcImg(i, j);
        }

    for (py::ssize_t i = 0; i < clbImage.shape(0); i++)
        for (py::ssize_t j = 0; j < clbImage.shape(1); j++)
        {
            if ((i == 0) || (j == 0) || (i == (clbImage.shape(0) - 1)) || (j == (clbImage.shape(1) - 1)))
            {
            }
            else
            {
                float calibrationValue = clbImage(i, j);

                const ssize_t neighbouringIndices[][2] = {
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
                    ssize_t total = 0;
                    ssize_t count = 0;
                    ssize_t mean = 0;
                    for (auto &indices : neighbouringIndices)
                    {
                        float neighbourgingCalibValue = clbImage(indices[0], indices[1]);

                        // check which pixels are 0 around the query pixel in the calibration image
                        if (neighbourgingCalibValue >= 0)
                        {
                            total += srcImg(indices[0], indices[1]);
                            count += 1;
                        }
                    }
                    // and use those to compute the average in the actual image}
                    if (total != 0)
                    {
                        mean = total / count;
                    }

                    // impute the pixel in the actual image
                    destImg(i, j) = static_cast<uint16_t>(mean);

                    // update the calibration image from negative to 0 for that index
                    clbImage(i, j) = 0;
                }
            }
        }
    return result;
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
