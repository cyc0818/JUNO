# Introduction
## What is JUNO distributed computing? 
JUNO is an international experiment, and resources are provided from several data centers around the world. The current JUNO sites include IN2P3, IHEP, JINR, CNAF, SDU…… 
DCI is a system which can integrate heterogeneous resources, hide complexity from users, and provide a simple way for users to use distributed resources with:
*  Global “ID” to identify each user
*  Unique interface for job submission and data access

![Fig. Diagram of `JUNO DCI`](figures/upload_b652a0450fb9f435604db7f473884cd5.png)

## Global authority and authentication system
*  User owns “ID” which consists of “X509 cert” + “VOMS info”, and submits  jobs or access data with “ID” 
   * X509 certificate contains user identity info
   * VO means "Virtral Organization", normally it presents an experiment
   * VOMS info contains user membership in JUNO VO  
*  All the systems and services in DCI can identify “ID” and do translations between “ID” and local accounts
*  When users use these IDs to submit jobs or get output, the systems can allow you to use services as local users. 

![Fig. Diagram of `Authority and Authentication system`](figures/upload_924219b12507149a34ccc39ea708ce99.png)

## How does DCI run your jobs?
The jobs will go through three layers in DCI

![Fig. Diagram of `workload management`](figures/upload_1c0f38f23fde6109338cd7b0fe835625.png)

### JSUB and Production system (ProdSys)
* Provide interface for physicist user or prod groups to split and submit tasks to DCI
### DIRAC 
* Interware between jobs and resources 
* Interface with local schedulers in each sites
* Receive jobs and schedule jobs to proper resources with pilot scheme
### Resource in sites
* Jobs reach and run in work nodes just as local computing do
## How do you use data in DCI?  
In distributed computing environment, data is located in different data centers, Data can’t be accessed locally

![Fig. Diagram of `data management`](figures/upload_2ff6647b9dee33b122727a395e3ba37b.png)

### Dirac File Catalogue (DFC)
* Interface with different SEs
Provide a global view and an interface to access remote data in a way similar to local data 
With LFN, data locations are transparent to users
### SE （Storage Element） 
File system where data is stored, with grid protocols for data movements between SEs 
Same files can have copies in > 1 SE
One LFN can map to > 1PFN
Normally users don’t need to directly visit SE
### FTS ( File Transfer System) 
Help massive data replication between SEs
## Official and individual usage
There are two user cases with different tools and interface
### Official production uses ProdSys (Production System)
* MC production need to share among groups or across different data centers
* Larger scale, standard pattern, need more space 
### Individual usage uses JSUB or DIRAC API
* Individual simulation, reconstruction or analysis
* Small scale, more flexible, can be customized
All output is available via DFC. In some sites such as IHEP, output available in your local cluster might also be directly accessible without DCI credentials
## How to apply official production?
### Official production is taken care by data production AFG
  * https://juno.ihep.ac.cn/mediawiki/index.php/Analysis_Foundation_Groups/Data_production
  * Consider to join the group
  * The DFC directory used by offline production is /juno/production
  * Disk space is negotiated through data production AFG
## Procedure for individual users 
  * Get Grid Certificate and join JUNO VO
  * Set up DCI environment and prepare proxy with cert
  * Use JSUB to create and submit jobs to the DCI
  * Check job status from JSUB or DIRAC web portal
  * Output is available from DFC
