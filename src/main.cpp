#include <iostream>
#include <math.h>
#include <functional>
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
    // copy srcImg to destImg
    for (py::ssize_t i = 0; i < srcClbImage.shape(0); i++)
        for (py::ssize_t j = 0; j < srcClbImage.shape(1); j++)
        {
            clbImage(i, j) = srcClbImage(i, j);
            destImg(i, j) = srcImg(i, j);
        }

    for (py::ssize_t i = 1; i < clbImage.shape(0) - 1; i++)
        for (py::ssize_t j = 1; j < clbImage.shape(1) - 1; j++)
        {

            float calibrationValue = clbImage(i, j);

            if (std::isless(calibrationValue, 0.0f))
            {
                const ssize_t neighbouringIndices[][2] = {
                    {i + 1, j},
                    {i + 1, j + 1},
                    {i + 1, j - 1},
                    {i - 1, j},
                    {i - 1, j + 1},
                    {i - 1, j - 1},
                    {i, j + 1},
                    {i, j - 1}};

                int32_t total = 0;
                int32_t count = 0;
                int32_t mean = 0;
                for (auto &indices : neighbouringIndices)
                {
                    float neighbourgingCalibValue = clbImage(indices[0], indices[1]);

                    // check which pixels are 0 or greater around the query pixel in the calibration image
                    if (neighbourgingCalibValue >= 0.0f)
                    {
                        total += destImg(indices[0], indices[1]);
                        count += 1;
                    }
                }
                // and use those to compute the average in the actual image}
                if (total != 0)
                {
                    mean = total / count;
                    // std::cout << "mean: " << mean << std::endl;
                    // std::cout << "total: " << total << std::endl;
                    // std::cout << "count: " << count << std::endl;
                }

                // impute the pixel in the actual image
                destImg(i, j) = static_cast<uint16_t>(mean);

                // update the calibration image from negative to 0 for that index
                clbImage(i, j) = 0.0f;
            }
        }
    return result;
}

PYBIND11_MODULE(pyoniip, m)
{
    m.doc() = R"pbdoc(
        pyoniip
        -----------------------
        .. currentmodule:: pyoniip
        .. autosummary::
           :toctree: _generate
           impute_image
    )pbdoc";

    m.def("impute_image", &impute_image, R"pbdoc(
        Pixel-wise mean imputation of an image

        This function takes as input two numpy arrays: 
         image[][]: uint16
         calibrationImage[][]: float

         of the same dimensions.
         The algorithm traverses the calibrationImage on a 
         pixel-by-pixel basis, looking for negative values. 
         A value average of nearest-neighbour (valid) pixels relative
         to the query pixel image[i,j] is then computed and imputed into
         image[i,j]. calibrationImage[i,j] is then set to 0.0 to indicate
         a corrected measurement. Array operations are not in-place 
         for the purpose of being explicit. 
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
