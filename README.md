# About

`libresolvconf` is C library that parses `resolv.conf` files, and is able to load ENVIROMENTAL variables into predefined structure. It uses Ragel to build automata that parses and partialy validates input, aiming for performance and reliability. It support more standards of `resolv.conf` files being compatible with `Linux` and `OpenBSD`.

# Build from source

It uses CMake build system.

1. Before build, install `ragel`, `cmake` and `gcc`. For example on debian based distributions 
```
sudo apt install -y ragel cmake gcc
```

2. Step into working directory and download source code from repository

```
cd <working-directory>
git clone https://github.com/xyz/
```

3. Create build directory

```
cd libresolvconf
mkdir build
```

4. Build from source code
```
cd build
cmake ..
make -j`nproc`
```
5. Optionaly you can install library into the system
```
sudo make install
```