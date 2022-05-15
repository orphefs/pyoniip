import pyoniip as m
import numpy as np

calibration_image_dtype = np.float32
image_dtype = np.uint16

calibration_image = np.array([[0., 0., 0., 0., 0., 0., 0., 0., 0.],
                              [0., 0., -1., -1., 0., 0., 0., 0., 0.],
                              [0., 0., -1., -1., 0., 0., 0., 0., 0.],
                              [0., 0., 0., 0., 0., 0., -1., -1., 0.],
                              [0., 0., 0., 0., 0., 0., 0., 0., 0.],
                              [0., 0., 0., 0., 0., 0., 0., 0., 0.]], dtype=calibration_image_dtype)

image = np.array([[1, 1, 1, 1, 1, 1, 1, 1, 1],
                  [1, 1, 2, 13, 1, 100, 1, 1, 1],
                  [1, 1, 3, 2, 1, 2, 1, 1, 1],
                  [1, 1, 1, 1, 1, 1, 1, 1, 1],
                  [1, 1, 1, 1, 1, 1, 1, 1, 1],
                  [1, 1, 1, 1, 1, 1, 1, 1, 1]], dtype=image_dtype)

expected_corrected_image = np.array([[1, 1, 1, 1, 1, 1, 1, 1, 1],
                                     [1, 1, 1, 1, 1, 100, 1, 1, 1],
                                     [1, 1, 1, 1, 1, 2, 1, 1, 1],
                                     [1, 1, 1, 1, 1, 1, 1, 1, 1],
                                     [1, 1, 1, 1, 1, 1, 1, 1, 1],
                                     [1, 1, 1, 1, 1, 1, 1, 1, 1]], dtype=image_dtype)


assert m.__version__ == '0.0.1'
assert np.all(m.impute_image(image, calibration_image)
              == expected_corrected_image)
