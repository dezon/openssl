# I Can Haz Proof?

Note: so far only tested on MacOS.

First of all, you need an old version of clang: 3.6.x.

You can find it [here](http://llvm.org/releases/download.html).

Then you need [SAW](http://saw.galois.com/downloads.html) and [Z3](https://github.com/Z3Prover/z3/releases).

Now, configure and build OpenSSL in a form useful to SAW:

    $ CC=<path to clang 3.6> ./config enable-saw
    $ make build_libs

Unfortunately, OpenSSL's build system is not currently flexible enough
to manage the next steps, so this may not work on all platforms
(patches welcome):

    $ cd proof
    $ SAW=<path to SAW binaries> Z3=<path to Z3 binaries> LLVM=<path the clang 3.6 binaries> make