# Operating Systems

Kernel for RISC V 64-bit architecture implemented without using any preexisting libraries. Supports First fit memory allocation, synchronous context switching and Semaphore objects.

## Usage

In test/userMain.cpp uncomment test function and its header file.
Start with command:

```
make clean
make qemu
```

## Make Commands

- clean - remove project compilation files
- all - compile project
- qemu - start kernel

## Dependencies

- build-essential
- qemu-system-misc
- gcc-riscv64-linux-gnu
- binutils-riscv64-linux-gnu
- gdb-multiarch
- g++-riscv64-linux-gnu

Or install using command:

```
sudo apt install build-essential qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu gdb-multiarch g++-riscv64-linux-gnu
```

Tested on Ubuntu 20.04.6 LTS

## To do

- create garbage collector
- lower memory usage of thread list and semaphore list
