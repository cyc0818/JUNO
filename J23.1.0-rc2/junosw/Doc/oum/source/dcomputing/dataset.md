# Dataset management
## DFC as metadata catalogue 
DFC (Dirac File Catalogue) not only can be used as a Replica Catalog, also can be used as a Metadata Catalog. This section will tell you how to define and query your dataset with DFC metadata. You can start to use DFC with the command: dirac-dms-file-catalogue
## DFC metadata
In the interactive console, the two commands "meta show" and "meta get" will help you know the exisiting metadata
* "meta show" to know what metadata indexes have been defined for files and directories
```
FC:/> meta show
      FileMetaFields : {'count': 'INT', 'status': 'INT', 'userdata': 'INT', 'eventNumber': 'INT', 'jobId': 'INT', 'userdata_s': 'VARCHAR(128)', 'runH': 'INT', 'runL': 'INT'}
 DirectoryMetaFields : {'test1': 'VARCHAR(128)', 'test0': 'INT', 'process': 'VARCHAR(128)', 'eventType': 'VARCHAR(30)', 'vo': 'VARCHAR(128)', 'tag': 'VARCHAR(128)', 'dyb_transfer': 'VARCHAR(128)', 'transID': 'VARCHAR(128)', 'decayCard': 'VARCHAR(4000)', 'application': 'VARCHAR(128)', 'bossVer': 'VARCHAR(10)', 'dirName': 'VARCHAR(128)', 'description': 'VARCHAR(128)', 'jobOptions': 'VARCHAR(4000)', 'dataType': 'VARCHAR(30)', 'junoVersion': 'VARCHAR(128)', 'position': 'VARCHAR(128)', 'particle': 'VARCHAR(128)', 'juno_transfer_temp': 'VARCHAR(128)', 'juno_transfer': 'VARCHAR(128)', 'softwareVersion': 'VARCHAR(128)', 'streamId': 'VARCHAR(10)', 'resonance': 'VARCHAR(30)', 'round': 'VARCHAR(30)'}
 ```
* "meta get" to know what metadata have been defined for the directory
```
FC:/>meta get /juno/production/radioactivity/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/C14
            !process : Chain
    *softwareVersion : centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0
                 *vo : juno
```
## Define dataset with metadata
* Define metadata index
Before using metadata, you need to define metadata first
```
meta index [-d|-f|-r] <metaname> [<metatype>]  - add new metadata index. Possible types are:
                      'int', 'float', 'string', 'date';
     -d  directory metadata
     -f  file metadata
     -r  remove the specified metadata index
```
For example:
```
meta index -d softwareVersion string
```
* Define metadata for the directories
With the metadata defined, you can use "meta set" to defind metadata for the directories
```
meta set  <directory> <metaname> <metavalue>
For example:

FC:/juno/user/z/zhangxm>meta set /juno/user/z/zhangxm/testdir test0 5
/juno/user/z/zhangxm/testdir {'test0': '5'}
FC:/juno/user/z/zhangxm>meta get /juno/user/z/zhangxm/testdir 
              !test0 : 5
```
* With the combination of enough metadata for the directories and files, you can choose the proper files for the dataset  
## Query dataset with metadata
After defining metadata for files and directories, you can find the specified dataset with the command "find"
```
FC:/juno/user/z/zhangxm>help find
find                : Find all files satisfying the given metadata information
                      
                      usage: find [-q] [-D] <path> <meta_name>=<meta_value> [<meta_name>=<meta_value>]
                      
```
For example, you define metadata test0=5 for the directory "/juno/user/z/zhangxm/testdir", you can find the files under this directory with metadata defined:
```
FC:/juno/user/z/zhangxm>find / test0=5
Query: {'test0': 5}
/juno/user/z/zhangxm/testdir/group3/prod.ini
/juno/user/z/zhangxm/testdir/group3/Yuri_prod.ini
/juno/user/z/zhangxm/testdir/group3/Yuri_prod_equator.ini
/juno/user/z/zhangxm/testdir/group0/test.root
/juno/user/z/zhangxm/testdir/group11/prod.ini
/juno/user/z/zhangxm/testdir/group11/Yuri_prod.ini
/juno/user/z/zhangxm/testdir/group11/Yuri_prod_equator.ini
/juno/user/z/zhangxm/testdir/group1/prod.ini
/juno/user/z/zhangxm/testdir/group1/Yuri_prod.ini
/juno/user/z/zhangxm/testdir/group1/Yuri_prod_equator.ini
/juno/user/z/zhangxm/testdir/group0/test_fcc.root
```
### Use dataset in ProdSys
In the steering file, you can set the inputQuery with metadata to find the proper dataset
inputQuery = {"transID":248,"test0":5}
