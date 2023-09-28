
# Install

## Build from source

This guide outlines the process of building the software from its source code using the CMake build system.

1. Prior to initiating the build, ensure that you have the necessary dependencies installed, including `cmake`, a C compiler (such as `gcc` or `clang`), `libcriterion-dev`, and `ragel`.

2. Navigate to your desired working directory and clone the source code repository:

```bash
cd <working directory>
git clone https://github.com/Payne-X6/libresolvconf.git
```

3. Create a dedicated build directory:

```bash
cd libresolvconf
mkdir build
```

4. Proceed to build the software from source:

```bash
cd build
cmake ..
make -j`nproc`
```

### Testing (Optional)

If you wish to run tests, use the test target from the build directory:
```bash
make test
```

### Documentation (Optional)

To generate documentation, you can utilize the `doc` target from the build directory. However, be sure to install the required Python packages as specified in `docs/requirements.txt` beforehand. You can install them using `pip`, either within a virtual environment (`venv`) or directly into your system.

To install the necessary dependencies in a virtual environment:
```bash
python3 -m venv .env
. .env/bin/activate
pip install -r docs/requirements.txt
```
Alternatively, if you prefer a system-wide installation:
```bash
pip install --user -r docs/requirements.txt
```
After satisfying the dependencies, proceed to build the documentation:
```bash
make doc
```

### Installation (Optional)

If you choose to, you can use the following command to perform an installation, which will include both the library and its associated binaries:
```bash
sudo make install
```