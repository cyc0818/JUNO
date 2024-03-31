# Appendix

In this part, several useful software and tools will be introduced briefly.

## `wiki`

* JUNO offline wiki page: <http://juno.ihep.ac.cn/mediawiki/index.php/Offline_Software>
* Useful links at getting started: <http://juno.ihep.ac.cn/mediawiki/index.php/Offline:GettingStarted>

## `trac`: a project management and bug/issue tracking system

* JUNO trac URL: <http://juno.ihep.ac.cn/trac>
* You could use trac:
    * read code online: <http://juno.ihep.ac.cn/trac/browser>
    * report bugs: <http://juno.ihep.ac.cn/trac/newticket>
    * view history: <http://juno.ihep.ac.cn/trac/timeline>

## `svn`: a version control system

* JUNO offline svn URL: <http://juno.ihep.ac.cn/svn/offline>
* private repository for user and group: <http://juno.ihep.ac.cn/svn/juno/>
    * Please don't upload binaries or big files, such as root file.

Useful commands are listed below:

Checkout code: 

    $ svn co http://juno.ihep.ac.cn/svn/offline/trunk offline

Commit changes:

    $ svn ci -m "some commit message you need to write."

Show status of current working directory:

    $ svn st
    $ svn st -q

Show your current modification:

    $ svn diff

Add a new file:

    $ svn add ./path/to/file

Add a new directory without files contained:

    $ svn add --depth=empty somedir

Show logs (last commit):

    $ svn log -l 1

## `cmt`: configuration management tool

* URL: <http://www.cmtsite.net/>
* It helps users:
    * build software and handle dependencies (**Makefile**, `macro` related in `requirements` file)
    * manage runtime environment (**setup.sh**, `setup` or `path` related syntax in `requirements` file)

Useful commands are listed below:

Check `$CMTPROJECTPATH`. Make sure the order is right.

    $ echo $CMTPROJECTPATH 
    /home/ihep/juno-mydev:/home/ihep/juno-dev:

Generate setup script and Makefile. It is necessary when you move package, add new files and so on.

    $ cmt config

Build the package. It is convenient when you are in `src` directory (no Makefile).

    $ cmt make

Build the whole packages.

    $ cmt br cmt config
    $ source setup.sh
    $ cmt br cmt make

When there is compilation or linking problem, please try to print more information:

    $ cmt make VERBOSE=1

You should check the macros used in Makefile are correct. Such as there is a macro called `ROOT_linkopts`:

    $ cmt show macros
    $ cmt show macro ROOT_linkopts 
    $ cmt show macro_value ROOT_linkopts

## `junoenv`: deploy JUNO offline software
`junoenv` is collection of tools to deploy JUNO offline software. It could:

* Install external libraries.
* Create setup scripts for installation.
* Install external interface packages.
* Install `SNiPER` framework.
* Install `offline` software.
* Download `offline-data`.

Before install the software, please make sure you are installing in a Linux system. Scientific Linux 6 is recommended.

It is recommended to have a clean environment. 

Let's start. 

Set environment `$JUNOTOP`. All the software will be installed under this directory.

    $ export JUNOTOP=/home/juno/JUNO-SOFT

Get `junoenv`:

    $ cd $JUNOTOP
    $ svn co http://juno.ihep.ac.cn/svn/offline/trunk/installation/junoenv
    $ cd junoenv

If you have `sudo` permission, you could run following command to install some dependencies:

    $ bash junoenv preq

Because the external libraries we depended will be changed, so use following command to show all the necessary libraries.

    $ bash junoenv libs list
    ==== juno-ext-libs-list: [ ] python@2.7.6 
    ==== juno-ext-libs-list: [ ] boost@1.55.0 -> python
    ==== juno-ext-libs-list: [ ] cmake@2.8.12.1 
    ==== juno-ext-libs-list: [ ] git@1.8.4.3 
    ==== juno-ext-libs-list: [ ] gccxml@master -> cmake
    ==== juno-ext-libs-list: [ ] xercesc@3.1.1 
    ==== juno-ext-libs-list: [ ] qt4@4.5.3 -> python boost
    ==== juno-ext-libs-list: [ ] gsl@1.16 
    ==== juno-ext-libs-list: [ ] fftw3@3.2.1 
    ==== juno-ext-libs-list: [ ] cmt@v1r26 
    ==== juno-ext-libs-list: [ ] clhep@2.1.0.1 
    ==== juno-ext-libs-list: [ ] ROOT@5.34.11 -> python boost cmake +git gccxml xercesc +qt4 gsl fftw3
    ==== juno-ext-libs-list: [ ] hepmc@2.06.09 
    ==== juno-ext-libs-list: [ ] geant4@9.4.p04 -> python boost cmake xercesc +qt4 clhep ROOT
    ==== juno-ext-libs-list: [ ] libmore@0.8.3 
    ==== juno-ext-libs-list: [ ] libmore-data@20140630 -> libmore

Then you could install external libraries one by one:

    $ bash junoenv libs all python
    $ bash junoenv libs list
    ==== juno-ext-libs-list: [x] python@2.7.6 
    ==== juno-ext-libs-list: [ ] boost@1.55.0 -> python
    ==== juno-ext-libs-list: [ ] cmake@2.8.12.1
    ...

    $ bash junoenv libs all boost
    ...
    $ bash junoenv libs all libmore
    $ bash junoenv libs all libmore-data

Please make sure every package is installed successfully. 

Then create runtime environment script:

    $ bash junoenv env

Install external interface:

    $ bash junoenv cmtlibs

Install `SNiPER`:

    $ bash junoenv sniper

Install `offline` and `offline-data`:

    $ bash junoenv offline
    $ bash junoenv offline-data
