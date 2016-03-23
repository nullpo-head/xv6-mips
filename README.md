This is MIPS port of Xv6, educational reimplementation of UNIX v6.  
Visit https://pdos.csail.mit.edu/6.828/2012/xv6.html for official information of Xv6.

This is written for MIPS R4000 and runs on qemu MIPS arch now.

## Building and Running

Xv6-mips is compiled with mipsel-sde-elf-gcc at current. You can build it very easily with crosstool-ng (http://crosstool-ng.org/). If you want to use other cross compilers, rewrite TOOLPREFIX in Makefile.

To build Xv6-mips, just run
```
$ make
```

Then Xv6-mips runs by
```
$ make qemu-memfs
```

It uses serial as its console, so use your terminal's standart io to communicate with Xv6 though the Qemu's empty black screen window appears.

## Original Xv6 README

This repository also includes the original Xv6 README. Please read "README" file for it.
