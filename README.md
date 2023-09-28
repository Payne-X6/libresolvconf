# About

`libresolvconf` is a C library that parses `resolv.conf` files and can load ENVIRONMENTAL variables into a predefined structure. It uses Ragel to build automata that parse and partially validate the input, while striving for performance and reliability. It supports multiple `resolv.conf` file standards and is syntax compatible under `Linux` and `OpenBSD`.

# Build from source

Uses the CMake build system.

1. Before building, install the `ragel`, `cmake`, and `gcc` dependencies. For example, on Debian-based distributions 

```
sudo apt install -y ragel cmake gcc
```

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