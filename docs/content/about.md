# About

`libresolvconf` is a C library that parses `resolv.conf` files and can load ENVIRONMENTAL variables into a predefined structure. It uses `ragel` to build FSM that parse and validate the input, while striving for performance and reliability. It also comes with utility `resolvconf-checker`, that check syntax of `resolv.conf` file.

It supports multiple `resolv.conf` file standards and is syntax compatible under `Linux` and `OpenBSD`. Tested platrofms are `Ubuntu` and `MacOS`.

