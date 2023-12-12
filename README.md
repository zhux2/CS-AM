# Digital Design and Computer Organization Experiments Project

This project is based on PA(the programming assignment of the class ICS(Introduction to Computer System)
in Department of Computer Science and Technology, Nanjing University).

## Hardware

Use Verilog HDL. 
Hardware component consists of three parts:
* CPU
* Memory(Pmem)
* I/O Device

Memory can be viewed as an array of 32-bit values, inplemented by uniting four 8-bit RAM. Code and data are both stored in it.

## Load code and data

A tool is designed to turn image file into four `.coe` files that can be load into Pmem.
To create `.coe` files, use following command under the specific directory:
```
CS-AM/am-kernels/tests/am-tests$ make image
```
The four `.coe` files will be in `/biuld`.

## More Information

Original PA, which is not fully implemented.
* [Abstract-Machine](https://github.com/NJU-ProjectN/abstract-machine)

Link to ICS PA:
* https://nju-projectn.github.io/ics-pa-gitbook/ics2023/
