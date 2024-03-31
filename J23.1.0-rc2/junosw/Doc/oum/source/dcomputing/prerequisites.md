# Prerequisites
## Apply a grid certificate
### Step1: visit https://cagrid.ihep.ac.cn to **Request a Certificate** from IHEP CA
It will take 2 or 3 days waiting for approval. When your application is approved, you will receive an email from ihepca@ihep.ac.cn with the serial number and DN. Then you can import the certificate to your browser, and then save it to local disk as a .p12 or .pfx file.
### Step2: generate userkey.pem and usercert.pem to $HOME/.globus
According to the guide in https://cagrid.ihep.ac.cn/html/export.html, with the .p12 file you can generate userkey.pem and usercert.pem with the following commands:
```
 openssl pkcs12 -in foo.p12 -out userkey.pem -nocerts
 openssl pkcs12 -in foo.p12 -out usercert.pem -nokeys -clcerts
```
Put these files to $HOME/.globus/ and then change their permission to 400 and 600.

When you save the .p12 file and generate .pem files, you will be informed to set password. Please write down your password somewhere for future use.

## Join JUNO VO

### Step1: Visit https://voms.ihep.ac.cn:8443/voms/juno/ to request a VO membership
With the certificate in web browser, you can go to https://voms.ihep.ac.cn:8443/voms/juno/ and follow the guidelines to join the JUNO VO. 
### Step2: Check with proxy generation
When it is approved, you will recieve a confirm mail. Use the command to generate temporary proxy: 
```
voms-proxy-init --voms juno
```
If it is successful, you will see the proxy /tmp/x509up_u*

```-bash-4.2$ voms-proxy-init --voms juno
Enter GRID pass phrase for this identity:
Contacting voms.ihep.ac.cn:15008 [/C=CN/O=HEP/OU=CC/O=IHEP/CN=voms.ihep.ac.cn] "juno"...
Remote VOMS server contacted succesfully.
Created proxy in /tmp/x509up_u23006.
Your proxy is valid until Thu Jun 24 03:19:03 CST 2021
```
## Register your certification and VO info in DIRAC
This step is automatically done by DIRAC itself, which means each 4 hours DIRAC will synchronize JUNO members from JUNO VOMS. If you are urgent to use DIRAC, you can contact the DIRAC admin zhangxuantong@ihep.ac.cn or zhangxm@ihep.ac.cn to do it by hand.

## Set up DIRAC env and start to use
### Set up DIRAC env with CVMFS
The DIRAC client is available in CVMFS. In login nodes with CVMFS installed, you can find DIRAC client and set up DIRAC env with source:
```
source /cvmfs/dcomputing.ihep.ac.cn/dirac/IHEPDIRAC/bashrc
```

Generate proxy with dirac-proxy-init, you can start to use DIRAC
``` 
dirac-proxy-init -g juno_user
```
    
### Install IHEPDIRAC to set up DIRAC env
If you don't have nodes with CVMFS installed, you can install IHEPDIRAC client with the following scripts:
```
#!/bin/bash
curl -LO https://github.com/DIRACGrid/DIRACOS2/releases/latest/download/DIRACOS-Linux-$(uname -m).sh
bash DIRACOS-Linux-$(uname -m).sh
rm DIRACOS-Linux-$(uname -m).sh
source diracos/diracosrc
pip install IHEPDIRAC
## Configuration server: dips://prod-dirac.ihep.ac.cn:9135/Configuration/Server
dirac-configure
```
    
For more information, please refer to     https://juno.ihep.ac.cn/mediawiki/index.php/JUNO_Distribute_Computing_User_Guide
