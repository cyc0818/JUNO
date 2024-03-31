# Detector Simulation Quick start (Development Environment)
For some advanced users and developers, it is not enough to control simulation by python script, we often need to add new functions or extend functions. A development environment is necessary when you compile the code and load the new functions into detector simulation. In this part, we will introduce how to setup a development environment for our detector simulation software. However, it is also useful for other software development.

## Basics
Before deploy your own environment, we need to know some basic knowledge. It is not required, but it would help you deploy our environment or build software more easily.

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

### CMT: Configuration Management Tool 
You have already learned how to build and use a library and how to run this example. Now we know, we need to take care of software compilation and runtime environment setup. For high energy physicists, we need a tool to simplify such jobs.

> CMT is a configuration management environment, based on some management conventions and comprises several shell-based utilities. It is an attempt to formalize software production and especially configuration management around a package-oriented principle. 

You can find more information in <http://www.cmtsite.net/>.

In this section, we will learn some basic commands:

* CMT helps you create Makefile and setup script from a file called `requirements`
* `cmt config`, create `setup.sh` and `Makefile`. **Note**, when you add a new file, please `cmt config` to force CMT generate Makefile again.
* `cmt make`, invoke `make` command and build current package.

Let's create a new package:

    $ cmt create MyPkg v0
    ------------------------------------------
    Configuring environment for package MyPkg version v0.
    CMT version v1r26.
    Root set to /home/ihep/juno-tutorial/LLR-Workshop.
    System is Linux-x86_64
    ------------------------------------------
    Installing package directory
    Version directory will not be created due to structuring style
    Installing cmt directory
    Installing src directory
    Creating setup scripts.
    Creating cleanup scripts.

Then copy previous code into `MyPkg/src`.

    $ cp ../MyFunc.cc src/
    $ cp ../MyFunc.hh src/
    $ cp ../main.cc src/

Then modify `MyPkg/cmt/requirements`:

    package MyPkg

    macro_prepend cppflags "-fPIC "
    
    library MyLib MyFunc.cc
    
    application main main.cc
    macro_append main_use_linkopts " -L$(bin) -lMyLib "
    macro_append main_dependencies MyLib
    
    path_prepend LD_LIBRARY_PATH $(bin)
    path_prepend PATH $(bin)

* `package` tells CMT the package name.
* `macro` in CMT is actually `macro` in Makefile. You can think them as variables used in Makefile. In `cmt` directory, you could use `cmt show macros` to show all defined macros.
* `library` tells CMT to create a shared library.
* `application` tells CMT to create a standalone executable.
* `path_prepend` will change the environment variables when you source setup.sh.

Then:

    $ cd MyPkg/cmt
    $ cmt config
    $ source setup.sh
    $ cmt make

Finally, you could invoke `main.exe`:

    $ main.exe 
    magic here, your input number: 42

If you have any problems during `cmt make`, you can use `VERBOSE=1` to increase log level:

    $ cmt make VERBOSE=1

### CMT in JUNO offline 
In CMT, packages are organized into projects. To help CMT to find projects or packages, there are two important environment variables:

* **`CMTPROJECTPATH`**: where projects can be found. Recommended.
* **`CMTPATH`**: where individual project can be found.

You could check these two environment variables:

    $ echo $CMTPROJECTPATH 
    /home/ihep/juno-dev:
    $ echo $CMTPATH        
    /home/ihep/juno-dev/offline:/home/ihep/juno-dev/sniper:/home/ihep/juno-dev/ExternalInterface
    $ echo $CMTPATH | tr ':' '\n'
    /home/ihep/juno-dev/offline
    /home/ihep/juno-dev/sniper
    /home/ihep/juno-dev/ExternalInterface

In our JUNO offline setup script, we only need to set `$CMTPROJECTPATH`. After we source `cmt/setup.sh` under specific package in one project, this project path will be prepended to `$CMTPATH`.

Here, **the order is important** again. CMT will use the first found package. For example, we need to find a package called `DummyPkg`. CMT will try to look for it in the first project. If the package is found, CMT will stop here. Otherwise, CMT will try to look for it in the next project, until CMT find it.

Actually, this order also shows you the dependencies of these software. The most fundamental project here is `ExternalInterface`, which supports interface packages, such as Python, Boost and so on. Then on the top it is `SNiPER` framework. Finally, `offline` is built upon `SNiPER`.

## Deploy your own environment
In previous sections, we already know some basics. So to deploy your own environment is easy now. What we need is to replace the official offline project with our own, while reuse `SNiPER` and other external libraries.

**Please note**, before we start, please make sure the environment variables are not polluted by other environment. You need a clean shell without sourcing any JUNO official setup script.

In this example:

* the official JUNO software (**`$JUNOTOP`**) is `/home/ihep/juno-dev`.
* the working directory (**`$WORKTOP`**) is `/home/ihep/juno-mydev`.

### prepare setup script
Under your working directory, let's create a setup script called `bashrc`:

    export JUNOTOP=/home/ihep/juno-dev
    export WORKTOP=/home/ihep/juno-mydev
    
    export JUNO_OFFLINE_OFF=1 # disable official offline before source setup.sh
    
    source $JUNOTOP/setup.sh

    export CMTPROJECTPATH=$WORKTOP:$CMTPROJECTPATH # prepend WORKTOP 
    
    if [ -d "$WORKTOP/offline/Examples/Tutorial/cmt/" ]; then
        pushd $WORKTOP/offline/Examples/Tutorial/cmt
        source setup.sh
        popd
    fi

The order is important. We need to set `JUNO_OFFLINE_OFF` before source official setup script. Otherwise the official offline will be loaded. 

Then you could source it:

    $ source bashrc
    $ echo $CMTPROJECTPATH 
    /home/ihep/juno-mydev:/home/ihep/juno-dev:
    echo $CMTPATH
    /home/ihep/juno-dev/sniper:/home/ihep/juno-dev/ExternalInterface

Because we don't checkout our own offline software yet, it is not listed in `$CMTPATH`.

### check out offline software

Under your working directory, just type:

    $ svn co http://juno.ihep.ac.cn/svn/offline/trunk offline

* `svn` is the source code control tool. 
* `co`, checkout.
* then is the SVN URL of our offline source code.
* `offline` is the local path name.

### compile and build offline software

Now let's compile and setup our own offline:

    $ cd offline/Examples/Tutorial/cmt
    $ cmt br cmt config
    $ source setup.sh
    $ cmt br cmt make

Here, most commands you should already know. I will explain what is `cmt br`. `br` means broadcast, it will help you invoke the same command repeatedly in all packages. So if you have about 100 used packages, you don't need to `cmt config` in every package.

### use your environment in next time
If you already build your own offline software, in the next time, you just `source bashrc`.
