# About

`libresolvconf` is a C library that parses `resolv.conf` files and can load ENVIRONMENTAL variables into a predefined structure. It uses Ragel to build FSM that parse and validate the input, while striving for performance and reliability. It also comes with utility `resolvconf-checker`, that check syntax of `resolv.conf` file.

It supports multiple `resolv.conf` file standards and is syntax compatible under `Linux` and `OpenBSD`. Tested platrofms are `Ubuntu` and `MacOS`.

# Install

## Build from source

Uses the CMake build system.

1. Before building, install the `cmake`, C compiler(`gcc`, `clang`, ...), `libcriterion-dev` and `ragel` dependencies.

2. Access the working directory and download the source code from the repository

```
cd <working directory>
git clone https://github.com/Payne-X6/libresolvconf.git
```

3. Create a build directory

```
cd libresolvconf
mkdir build
```

4. Build from source

```
cd build
cmake ..
make -j`nproc`
```

5. Optionally, you can install the library

```
sudo make install
```