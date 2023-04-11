# Memory Manager

This repository contains a memory manager implementation that provides an interface for allocating and deallocating memory blocks.


## Purpose

The purpose of this memory manager implementation is to efficiently manage memory which includes keeping track of which parts of memory are in use, allocating memory to processes, and freeing up the memory after the processes have used it. In other words, it helps manage memory more efficiently and provides additional features such as flushing out unused segments of memory.

## Main Files

- manager_test.c
- manager.c
- manager.h

## Usage

To use this memory manager implementation in your project, include `manager.h` in your source code and link against `manager.c`.

## dslib

`dslib` is a library of data structures that are used by the memory manager implementation.
`dslib.c` contains the implementation of the data structures while `dslib.h` contains the declarations.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
