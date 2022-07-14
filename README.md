# Modbus conv float

This application converts floating point numbers to use them with stdin_to_modbus_shm

## Build
```
mkdir build
cd build
cmake .. -DCMAKE_CXX_COMPILER=$(which clang++) -DCMAKE_BUILD_TYPE=Release -DCLANG_FORMAT=OFF -DCOMPILER_WARNINGS=OFF
cmake --build . 
```

## Use
```
modbus_conv_float
  Converts float values to use them with stdin_to_modbus_shm.
  If line starts with 'f:', the 'f:' is removed and the float value is converted to two hex registers.
  All other lines are passed through.

  example:
    input : f:ao:0xcf:123.4
    output: ao:0xcf:0x42f6
            ao:0xd0:0xcccd
```

