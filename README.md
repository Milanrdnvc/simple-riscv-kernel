# Operating Systems 1 - Project

## University of Belgrade, Faculty of Electrical Engineering

### Course: Operating Systems 1

Welcome to the repository for my Operating Systems 1 project. This project is developed as part of the coursework at the University of Belgrade, Faculty of Electrical Engineering.

## Project Overview

This project involves developing a basic operating system kernel that supports threads with time-sharing. The primary focus is on the core functionalities of the operating system, such as thread management, memory management, and handling system calls.

### Key Features:
- **Thread Management**: Implementation of context switching (synchronous and asynchronous - timer), scheduling, and thread synchronization.
- **Memory Management**: Basic memory allocation and deallocation, using contiguous memory allocation.
- **System Calls**: Handling essential system calls for memory management, threads, semaphores and console I/O operations.
- **Console Buffering**: Efficient handling of console I/O with buffer management.

## Installation

You can clone this repository to your local development environment. There's a makefile script included in the project that's used to compile and build it. You will need to [download a virtual machine](os.etf.rs/OS1/projekat/project-base-v1.1.zip) that has all the necessary tools installed in order to be able to run the project. This includes QEMU emulator for emulating a RISC-V CPU as well as an IDE, but you can use any IDE you want.

Run the project:
```bash
make qemu
```
Clean the project:
```bash
make clean
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
