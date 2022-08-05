# STDIN to Modbus SHM float converter

This application can be used together with
[stdin-to-modbus-shm](https://nikolask-source.github.io/stdin_to_modbus_shm/)
to write float values into the shared memory of a shared memory modbus client (
[TCP](https://nikolask-source.github.io/modbus_tcp_client_shm/) / [RTU](https://nikolask-source.github.io/modbus_rtu_client_shm/) ).


## Use the Application
The application is used in combination with stdin-to-modbus-shm:
```
modbus-conv-float | stdin-to-modbus-shm
```

It accepts the same commands as [stdin-to-modbus-shm](https://nikolask-source.github.io/stdin_to_modbus_shm/) and forwards them.

In addition, the application accepts commands that start with ```f:```. 
These indicate a float value that is to be converted to two modbus registers.
Conversion is only possible for the modbus registers AI and AO.

### Example commands
Some commands that will be forwarded:
```
do:0xA:1
ao:0xA:0xAA55
ai:0x0:0
```

Some commands that will be converted:
```
f:ao:0xF:3.141
f:ai:0xAFC:-42.0
```

## Using the Flatpak package
The flatpak package can be installed via the .flatpak file.
This can be downloaded from the GitHub [projects release page](https://github.com/NikolasK-source/modbus_conv_float/releases):

```
flatpak install --user modbus-conv-float.flatpak
```

The application is then executed as follows:
```
flatpak run network.modbus-conv-float
```

To enable calling with ```modbus-conv-float``` [this script](https://gist.github.com/NikolasK-source/fabfb771fe828d658845d8a2d79817e6) can be used.
In order to be executable everywhere, the path in which the script is placed must be in the ```PATH``` environment variable.


## Build from Source

The following packages are required for building the application:
- cmake
- clang or gcc

Use the following commands to build the application:
```
git clone https://github.com/NikolasK-source/modbus_conv_float.git
cd modbus_conv_float
mkdir build
cmake -B build . -DCMAKE_BUILD_TYPE=Release -DCLANG_FORMAT=OFF -DCOMPILER_WARNINGS=OFF
cmake --build build
```

The binary is located in the build directory.


## Links to related projects

### General Shared Memory Tools
- [Shared Memory Dump](https://nikolask-source.github.io/dump_shm/)
- [Shared Memory Write](https://nikolask-source.github.io/write_shm/)
- [Shared Memory Random](https://nikolask-source.github.io/shared_mem_random/)

### Modbus Clients
- [RTU](https://nikolask-source.github.io/modbus_rtu_client_shm/)
- [TCP](https://nikolask-source.github.io/modbus_tcp_client_shm/)

### Modbus Shared Memory Tools
- [STDIN to Modbus](https://nikolask-source.github.io/stdin_to_modbus_shm/)
- [Float converter](https://nikolask-source.github.io/modbus_conv_float/)


## License

MIT License

Copyright (c) 2021-2022 Nikolas Koesling

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
