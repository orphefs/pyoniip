pyoniip
==============

|      CI              | status |
|----------------------|--------|
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |
| [`cibuildwheel`][]   | [![Wheels Actions Status][actions-wheels-badge]][actions-wheels-link] |

[actions-badge]:           https://github.com/orphefs/pyoniip/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/orphefs/pyoniip/actions?query=workflow%3A%22Conda
[actions-conda-badge]:     https://github.com/orphefs/pyoniip/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/orphefs/pyoniip/actions?query=workflow%3A%22Pip
[actions-pip-badge]:       https://github.com/orphefs/pyoniip/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/orphefs/pyoniip/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/orphefs/pyoniip/workflows/Wheels/badge.svg

A [pybind11](https://github.com/pybind/pybind11) module built with a
CMake-based build system.

Installation
------------

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/orphefs/pyoniip.git
pip install ./pyoniip
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.

CI Examples
------------

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [`cibuildwheel`][]. You can also see a basic recipe for building and
testing in `pip.yml`, and `conda.yml` has an example of a conda recipe build.

Building the documentation
------------

Documentation for the example project is generated using Sphinx. Sphinx has the
ability to automatically inspect the signatures and documentation strings in
the extension module to generate beautiful documentation in a variety formats.
The following command generates HTML-based reference documentation; for other
formats please refer to the Sphinx manual:

- `cd cmake_example/docs`
- `make html`

License
-------

pyoniip is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

Tests
---------

First, instatiate a local `venv` environment:

```bash
python -m venv venv
source venv/bin/activate
pip install -r unpinned_requirements.txt
```

Then you can run the tests:

```bash
python tests/test.py
```

## Usage

You can import and use the module like so:

```python
import pyoniiip
imputed_image = pyoniip.impute_image(image, calibration_image)
```

where

```python
image: npt.typing.NDArray[np.uint16]
calibration_image: npt.typing.NDArray[np.float]
imputed_image: npt.typing.NDArray[np.uint16]
```

[`cibuildwheel`]:          https://cibuildwheel.readthedocs.io
