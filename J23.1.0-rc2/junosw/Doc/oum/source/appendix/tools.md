# Tools

In this part, several useful software and tools will be introduced briefly.

## `wiki`

* JUNO offline wiki page: <https://juno.ihep.ac.cn/mediawiki/index.php/Offline_Software>
* Useful links at getting started: <https://juno.ihep.ac.cn/mediawiki/index.php/Offline:GettingStarted>

## `gitlab`: project management and bug/issue tracking system

* JUNOSW official repository on IHEP GitLab: <https://code.ihep.ac.cn/JUNO/offline/junosw>
  * issues: <https://code.ihep.ac.cn/JUNO/offline/junosw/-/issues>
  * merge requests: <https://code.ihep.ac.cn/JUNO/offline/junosw/-/merge_requests>

## `git`: a version control system

* JUNOSW official URL:
  * SSH (preferred): git@code.ihep.ac.cn:JUNO/offline/junosw.git
  * HTTPS: <https://code.ihep.ac.cn/JUNO/offline/junosw.git>
* Development model:
  * Developers should fork the official repository and develop in their own repository
  * When the code is ready, developers should create a merge request (MR)
  * Code should be reviewed in the MR. 

### SSH Key Management

Generate a SSH key if it does not exist. It will generate a private and public key pair. 
You should set a password for security. An example:

    $ ls $HOME/.ssh/id_rsa # if exists, then skip the following step
    $ ssh-keygen
    
Copy the public key and register it into IHEP GitLab

    $ cat $HOME/.ssh/id_rsa.pub # copy the output
    
Visit <https://code.ihep.ac.cn/-/profile/keys> and add your public key. 

Check the key is registered. Maybe you will be asked to enter the password of the private key. 

    $ ssh -T git@code.ihep.ac.cn
    Welcome to GitLab, @lintao!
    
As the private key is protected with password, we need to enter password every time. 
We can use ssh-agent to manage the private key.

    $ eval $(ssh-agent) # start an ssh-agent
    $ ssh-add           # register your private key
    $ ssh-add -L        # check the key is registered
    
You can check with ssh again.

### Git Commands

Tell git who you are:

    $ git config --global user.name "Your Name Comes Here"
    $ git config --global user.email you@yourdomain.example.com

Clone code from official repository:

    $ git clone git@code.ihep.ac.cn:JUNO/offline/junosw.git
    
Clone code from user repository (you should fork the official repository):

    $ git clone git@code.ihep.ac.cn:YOURNAME/junosw.git

Add additional repository (`mine` is an alias name, then following the user repository):

    $ git remote add mine git@code.ihep.ac.cn:YOURNAME/junosw.git

Sync local repository and remote repository (sync the local main branch):

    $ git push mine main # push your local changes to a branch in remote repository
    $ git pull origin main # merge the remote branch main into local repository

Switch branch:

    $ git checkout main          # switch to an existing branch
    $ git checkout -b new-branch # creatie a new branch
    $ git branch -d new-branch   # delete a branch

Commit changes in local repository:

    $ git commit
    $ git commit -m "some commit messages you need to write"
    
Show status of current working directory:

    $ git status
    
Show modification:
    
    $ git diff
    
Add a file:

    $ git add ./path/to/file
    
Show logs (last commit):

    $ git log -1

## `trac`: a project management and bug/issue tracking system (legacy)

* JUNO trac URL: <https://juno.ihep.ac.cn/trac>
* You could use trac:
    * read code online: <https://juno.ihep.ac.cn/trac/browser>
    * report bugs: <https://juno.ihep.ac.cn/trac/newticket>
    * view history: <https://juno.ihep.ac.cn/trac/timeline>

## `svn`: a version control system (legacy)

* JUNO offline svn URL: <https://juno.ihep.ac.cn/svn/offline>
* private repository for user and group: <https://juno.ihep.ac.cn/svn/juno/>
    * Please don't upload binaries or big files, such as root file.

Useful commands are listed below:

Checkout code: 

    $ svn co https://juno.ihep.ac.cn/svn/offline/trunk offline

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

Before install the software, please make sure you are installing in a Linux system. CentOS 7 is recommended.

It is recommended to have a clean environment. 

Let's start. 

Set environment `$JUNOTOP`. All the software will be installed under this directory.

    $ export JUNOTOP=/home/juno/JUNO-SOFT

Get `junoenv`:

    $ cd $JUNOTOP
    $ git clone git@code.ihep.ac.cn:JUNO/offline/junoenv.git
    $ cd junoenv

If you have `sudo` permission, you could run following command to install some dependencies:

    $ bash junoenv preq

We are using modern C++, so a modern compiler is needed to build the software stack.

    $ source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/binutils/2.36/bashrc
    $ source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bashrc

If you don't have any modern compilers, you can install it using the script `dev-toolsets/collection_binutils236_gcc1120.sh`.

Because the external libraries we depended will be changed, so use following command to show all the necessary libraries.

    $ bash junoenv libs list
    ==== juno-ext-libs-list: [ ] git@2.37.3
    ==== juno-ext-libs-list: [ ] cmake@3.24.1
    ==== juno-ext-libs-list: [ ] python@3.9.14
    ==== juno-ext-libs-list: [ ] python-setuptools@58.1.0 -> python
    ==== juno-ext-libs-list: [ ] python-pip@22.2.2 -> python python-setuptools
    ==== juno-ext-libs-list: [ ] python-cython@0.29.28 -> python
    ==== juno-ext-libs-list: [ ] python-numpy@1.22.3 -> python python-setuptools python-cython
    ==== juno-ext-libs-list: [ ] libyaml@0.2.4
    ==== juno-ext-libs-list: [ ] python-yaml@5.4.1.1 -> python python-cython libyaml
    ==== juno-ext-libs-list: [ ] boost@1.78.0 -> python python-numpy
    ==== juno-ext-libs-list: [ ] xercesc@3.2.3
    ==== juno-ext-libs-list: [ ] gsl@2.7
    ==== juno-ext-libs-list: [ ] fftw3@3.3.10
    ==== juno-ext-libs-list: [ ] sqlite3@3.38.5
    ==== juno-ext-libs-list: [ ] mysql-connector-c@6.1.9 -> cmake
    ==== juno-ext-libs-list: [ ] mysql-connector-cpp@1.1.12 -> boost cmake mysql-connector-c
    ==== juno-ext-libs-list: [ ] pacparser@1.3.7
    ==== juno-ext-libs-list: [ ] frontier@2.9.1 -> pacparser
    ==== juno-ext-libs-list: [ ] log4cpp@1.1.3
    ==== juno-ext-libs-list: [ ] libxml2@2.9.12 -> python
    ==== juno-ext-libs-list: [ ] LHAPDF@6.3.0 -> python
    ==== juno-ext-libs-list: [ ] pythia6@6.4.28 -> python
    ==== juno-ext-libs-list: [ ] tbb@2020.3 -> cmake
    ==== juno-ext-libs-list: [ ] clhep@2.4.1.0 -> cmake
    ==== juno-ext-libs-list: [ ] xrootd@5.4.3 -> python cmake
    ==== juno-ext-libs-list: [ ] ROOT@6.26.06 -> python boost cmake +git +gccxml xercesc +qt4 gsl fftw3 tbb xrootd +sqlite3 pythia6
    ==== juno-ext-libs-list: [ ] hepmc@2.06.11
    ==== juno-ext-libs-list: [ ] geant4@10.04.p02.juno -> python boost cmake xercesc +qt4 +clhep ROOT
    ==== juno-ext-libs-list: [ ] genie@3.02.00 -> python log4cpp libxml2 LHAPDF pythia6 ROOT
    ==== juno-ext-libs-list: [ ] nuwro@19.02.2 -> libxml2 pythia6 ROOT
    ==== juno-ext-libs-list: [ ] talys@1.95 -> pythia6 ROOT
    ==== juno-ext-libs-list: [ ] gibuu@r2021_01 -> python LHAPDF pythia6 ROOT
    ==== juno-ext-libs-list: [ ] libmore@0.8.3
    ==== juno-ext-libs-list: [ ] libmore-data@20140630 -> libmore
    ==== juno-ext-libs-list: [ ] libonnxruntime@1.11.1 -> python cmake

Then you could install external libraries one by one:

    $ bash junoenv libs all git
    $ bash junoenv libs all cmake
    $ bash junoenv libs all python
    $ bash junoenv libs list
    ==== juno-ext-libs-list: [x] git@2.37.3
    ==== juno-ext-libs-list: [x] cmake@3.24.1
    ==== juno-ext-libs-list: [x] python@3.9.14
    ...

    $ bash junoenv libs all boost
    ...
    $ bash junoenv libs all libmore
    $ bash junoenv libs all libmore-data

Please make sure every package is installed successfully. 

Then create runtime environment script:

    $ bash junoenv env

Install `SNiPER`:

    $ bash junoenv sniper

Install `offline` and `offline-data`:

    $ bash junoenv offline
    $ bash junoenv offline-data
