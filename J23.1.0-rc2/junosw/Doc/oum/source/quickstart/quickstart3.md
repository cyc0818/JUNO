# Quick start (Development Environment)
For some advanced users and developers, it is not enough to just configure using python script, we often need to add new functions or extend functions. A development environment is necessary when you compile the code and load the new functions into the software. In this part, we will introduce how to setup a development environment. However, it is also useful for other software development.


## Deploy your own development environment (NEW: CMake is used)

Let's deploy your own development environment now. What we need is to replace the official offline project with our own, while reuse `SNiPER` and other external libraries.

**Please note**, before we start, please make sure the environment variables are not polluted by other environment. You need a clean shell without sourcing any JUNO official setup script.

In this example:

* the official JUNO software (**`$JUNOTOP`**) is `/home/ihep/juno-dev`.
* the working directory (**`$WORKTOP`**) is `/home/ihep/juno-mydev`.


### prepare setup script

Under your working directory, let's create a setup script called `bashrc`:

    export JUNOTOP=/home/ihep/juno-dev
    export WORKTOP=/home/ihep/juno-mydev
    
    export JUNO_OFFLINE_OFF=1 # disable official offline before source setup.sh

    pushd $JUNOTOP >& /dev/null
    source setup.sh
    popd >& /dev/null

    pushd $WORKTOP >& /dev/null
    if [ -f junosw/InstallArea/setup.sh" ]; then
        echo 'Setup Local JUNOSW (CMake version)'
        pushd junosw/InstallArea >& /dev/null
        source setup.sh
        popd >& /dev/null
    fi
    popd >& /dev/null

The order is important. We need to set `JUNO_OFFLINE_OFF` before source official setup script. Otherwise the official offline will be loaded. 

Then you could source it:

    $ source bashrc

### check out offline software

Under your working directory, just type:

    $ git clone git@code.ihep.ac.cn:JUNO/offline/junosw.git

* `git` is the source code control tool. 
* `clone`: clone the remote repository.
* the third is the URL of remote repository

### compile and build offline software

Now let's compile and setup our own offline:

    $ cd junosw
    $ ./build.sh

### use your environment in next time
If you already build your own offline software, in the next time, you just `source bashrc`.









## Understand underlying
In order to understand the internal when you deploy your own environment, we need to know some basic knowledge. It is not required, but it would help you deploy our environment or build software more easily.

### compilation and building process
A C++ written software could be running in an operating system when it is already built. The C++ source code could not be interpreted by operating system directly. We need to compile the source code into binary. In a Linux system, the binary could be a standalone executable, a shared library or a static library.

First, let's review what happens when you build your code. We have a collection of c++ headers and source code, which are edited by our editor, such as vim, emacs and so on. In this quick start, we need three files: `MyFunc.hh`, `MyFunc.cc` and `main.cc`

`MyFunc.hh`:

    #ifndef MyFunc_hh
    #define MyFunc_hh
    
    /*
     * magic function
     */
    void myfunc(int num);
    
    #endif

`MyFunc.cc`:

    #include "MyFunc.hh"

    #include <iostream>
    /*
     * Implement of magic function
     */
    void myfunc(int num) {
        std::cout << "magic here, your input number: " << num << std::endl;
    }

`main.cc`:

    #include "MyFunc.hh"
    
    int main() {
       myfunc(42);
    }

Then we need to compile the source code into object code.

    $ g++ -fPIC -c MyFunc.cc -I.

* Option `-fPIC` tells compiler to build code which is suitable for shared library. 
* Option `-c` tells compiler to compile the software.
* Option `-I` is **important**. It tells compiler where is the header files. Here `-I.` means look for header in current directory. Sometimes, you could also use environment variable **`$CPATH`** to tell compiler these directories. 

Finally, we could build our library.

    $ g++ -fPIC -shared -o libMyLib.so MyFunc.o

* Option `-shared` indicates building a shared library.
* Option `-o libMyLib.so` specifies the output file name.
* `MyFunc.o` is the input object file. We could input several object files at same time.

After we have our own library, let's use it.

    $ g++ -c main.cc -I.
    $ g++ -o example1 main.o -L. -lMyLib

* Option `-L` is same as `-I`, but specifying where is the shared libraries.
* Option `-l` specifies the linked library name.
* These two options could also be used when you building a shared library.

### runtime environment
Now we already have a shared library and an executable file. Let's try to run `example1`:

    $ ./example1 
    ./example1: error while loading shared libraries: libMyLib.so: cannot open shared object file: No such file or directory

But we already have `libMyLib.so`, why?

That's because building environment and runtime environment are different. Even though when building `example1` we already specify the shared library, when it is running, it needs to resolve where are the libraries it is linked. In a Linux system, you could use `ldd` to help you:

    $ ldd example1 
            linux-vdso.so.1 =>  (0x00007fffc8d28000)
            libMyLib.so => not found
            libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fdd46530000)
            /lib64/ld-linux-x86-64.so.2 (0x00007fdd46a00000)

As you see, it could not find `libMyLib.so`. So we need an **important** environment variable called **`$LD_LIBRARY_PATH`**.

    $ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
    $ ldd example1 
            linux-vdso.so.1 =>  (0x00007fffefb9f000)
            libMyLib.so => ./libMyLib.so (0x00007fc7685b0000)
            libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fc7681d0000)
            libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fc767eb0000)
            /lib64/ld-linux-x86-64.so.2 (0x00007fc768800000)
            libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fc767ba0000)
            libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fc767980000)

We prepend current directory to **`$LD_LIBRARY_PATH`**. The search order is important, it will use the first found library. Now let's run our example:

    $ ./example1
    magic here, your input number: 42

### another implementation
So you may ask why need such design? Let's explain it using an example.

Here is another source code `MyFunc2.cc`:

    #include "MyFunc.hh"
    
    #include <iostream>
    /*
     * Implement of magic function
     */
    void myfunc(int num) {
        std::cout << "magic here, your input number x2: " << (num*2) << std::endl;
    }

We need to create another library called `libMyLib.so` but in a different directory.

    $ mkdir v2
    $ g++ -fPIC -c MyFunc2.cc -I.
    $ g++ -fPIC -shared -o v2/libMyLib.so MyFunc2.o 

Now, a quite important step, we prepend `v2` to `$LD_LIBRARY_PATH`.

    $ export LD_LIBRARY_PATH=v2:$LD_LIBRARY_PATH
    $ ldd ./example1                            
            linux-vdso.so.1 =>  (0x00007fffd493a000)
            libMyLib.so => v2/libMyLib.so (0x00007fa1a4af0000)
            libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa1a4710000)
            libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fa1a43f0000)
            /lib64/ld-linux-x86-64.so.2 (0x00007fa1a4e00000)
            libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fa1a40e0000)
            libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fa1a3ec0000)
    $ ./example1
    magic here, your input number x2: 84

As you see, the result is changed. This is useful when you just want to replace several libraries.

But **please note**, this technique requires you **keep your function interface unchanged**. That's why we often say **keep interface and implementation separate**. If an object size in memory is changed and different from compilation stage, the program will crash.

**NOTES**: In our development environment, I don't recommend you to mix several different environment unless you really know what happens.

### make: a build automation tool
In previous examples, we need to type building commands in command line every time. If there is only one file, it could be OK. But if there are more files, how to do?

The answer is **Make**!

> GNU Make is a tool which controls the generation of executables and other non-source files of a program from the program's source files. Make gets its knowledge of how to build your program from a file called the makefile, which lists each of the non-source files and how to compute it from other files.

Let's write `Makefile` to build previous examples.

    all: main libMyLib.so
    
    main: main.o libMyLib.so
    	g++ -o example1 main.o -L. -lMyLib
    
    main.o: main.cc
    	g++ -c main.cc -I.
    
    libMyLib.so: MyFunc.o
    	g++ -fPIC -shared -o libMyLib.so MyFunc.o
    
    MyFunc.o: MyFunc.cc
    	g++ -fPIC -c MyFunc.cc -I.

* **Please note**, please replace the *spaces* with one *tab* in each line. Makefile requires *tab*.
* In makefile, we could specify the building dependencies. For example, `main.o` depends on `main.cc`, so if main.cc is modified later, `main.o` will be rebuild by Makefile again.
    
Then we could build the software by typing `make`:

    $ make
    g++ -c main.cc -I.
    g++ -fPIC -c MyFunc.cc -I.
    g++ -fPIC -shared -o libMyLib.so MyFunc.o
    g++ -o example1 main.o -L. -lMyLib

If you want to learn more about Make and makefile, you could read GNU make manual in <https://www.gnu.org/software/make/manual/make.html>.


### CMake in JUNO offline (CMake is used since J21)
In CMake, packages are organized into projects. To help CMake to find projects, there is one important environment variable:

* **`CMAKE_PREFIX_PATH`**: where projects can be found.



