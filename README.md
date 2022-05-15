pyoniip
==============

[![Gitter][gitter-badge]][gitter-link]

|      CI              | status |
|----------------------|--------|
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |
| [`cibuildwheel`][]   | [![Wheels Actions Status][actions-wheels-badge]][actions-wheels-link] |

[actions-badge]:           https://github.com/orphefs/pyoniip/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/orphefs/pyoniip/actions?query=workflow%3A%22Conda
[actions-conda-badge]:     https://github.com/orphefs/pyoniip/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/orphefs/pyoniip/actions?query=workflow%3A%22Pip
[actions-pip-badge]:       https://github.com/orphefs/pyoniip/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/orphefs/pyoniip/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/orphefs/pyoniip/workflows/Wheels/badge.svg

A project built with [pybind11](https://github.com/pybind/pybind11).
This requires Python 3.6+; for older versions of Python, check the commit
history.

Installation
------------

- clone this repository
- `pip install ./pyoniip`

CI Examples
-----------

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [`cibuildwheel`][]. You can also see a basic recipe for building and
testing in `pip.yml`, and `conda.yml` has an example of a conda recipe build.

Building the documentation
--------------------------

Documentation for the example project is generated using Sphinx. Sphinx has the
ability to automatically inspect the signatures and documentation strings in
the extension module to generate beautiful documentation in a variety formats.
The following command generates HTML-based reference documentation; for other
formats please refer to the Sphinx manual:

- `cd python_example/docs`
- `make html`

License
-------

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

Test call
---------

```python
import pyoniiip
pyoniip.impute_image(image: npt.typing.NDArray[np.uint16], calibration_image: npt.typing.NDArray[np.float])
```

[`cibuildwheel`]:          https://cibuildwheel.readthedocs.io
