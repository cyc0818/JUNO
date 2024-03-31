#! /usr/bin/env python

import os, sys, shutil

pathtoprec = sys.argv[1]
try:
    fsock = open(sys.argv[1], "r")
except IOError:
    print "The file don't exist, Please double check!"
    exit()
AllLines = fsock.readlines()
allcontain = str()
for EachLine in AllLines:
 EachLine.strip()
 if not len(EachLine) or EachLine.startswith('#') or EachLine.startswith('\n'):
  continue
 allcontain = allcontain + EachLine
fsock.close()
allcontain_tem = allcontain.split(';')
valid_num = len(allcontain_tem)
database_name = allcontain_tem[0].strip('\n').strip()
table_name = allcontain_tem[1].strip('\n').strip()
column_lis = list()
for index in range(2, valid_num):
 column_lis.append(allcontain_tem[index].strip('\n').strip())
column_valid = list()
column_num = len(column_lis)
column_pro_num = list()
temp = list()
for index in range(0, column_num):
 temp.append(column_lis[index].split('|'))
 column_pro_num.append(len(temp[index]))
 for index_su in range(0, len(temp[index])):
  column_valid.append(temp[index][index_su].strip('\n').strip())

pathtoclassfile_h = table_name + ".h"
pathtoclassfile_cc = table_name + ".cc"
pathtoclasslink_h = table_name + "LinkDef.h"

head_im = "//******************************************************************************\n"\
        + "//*NOTE: This file is automatically writen by script, please do not change it  *\n"\
        + "//*If you are an expert, you can ingore the note above                         *\n"\
        + "//******************************************************************************\n\n"
head_im = head_im\
        + "#ifndef " + table_name.upper() + "_H\n"\
        + "#define " + table_name.upper() + "_H\n\n"\
        + "#include " + '''"DatabaseSvc/DBITableRow.h"\n\n'''\
        + "class " + table_name + ": public DBITableRow\n"\
        + " {\n"\
        + "  public:\n"\
        + "  " + table_name + "(){}\n"\
        + "  virtual ~" + table_name + "(){}\n\n"
#define functions to finish the name, type, and description
name_lis = list()
type_lis = list()
name_index = 0
type_index = 1
for index in range(0, column_num):
 if(name_index < len(column_valid)):
  name_lis.append(column_valid[name_index])
  name_index = name_index + column_pro_num[index]

for index in range(0, column_num):
 if(type_index < len(column_valid)):
  type_lis.append(column_valid[type_index])
  type_index = type_index + column_pro_num[index]

head_im = head_im + "  private:\n"

for index in range(0, len(name_lis)):
 head_im = head_im + "  " + type_lis[index] + " f" + name_lis[index] + ";\n"

head_im = head_im + "\n  public:\n"

for i in range(0, len(name_lis)):
 head_im = head_im + "  void Set" + name_lis[i] + "(" + type_lis[i] + " " + name_lis[i] + ")"\
         + "{f" + name_lis[i] + " = " + name_lis[i] + ";}\n"

for i in range(0, len(name_lis)):
 head_im = head_im + "  " + type_lis[i] + " Get" + name_lis[i] + "()"\
         + "{return f" + name_lis[i] + ";}\n"

head_im = head_im + "\n  ClassDef(" + table_name + ", 1)\n"
head_im = head_im + "  };\n" + "#endif"

fclass = open(pathtoclassfile_h, 'w')
fclass.write(head_im)
fclass.close()

cc_im = "//******************************************************************************\n"\
        + "//*NOTE: This file is automatically writen by script, please do not change it  *\n"\
        + "//*If you are an expert, you can ingore the note above                         *\n"\
        + "//******************************************************************************\n\n"\
        + "#include " + '''"''' + pathtoclassfile_h + '''"\n'''\
        + "ClassImp(" + table_name + ");"
fcc = open(pathtoclassfile_cc, 'w')
fcc.write(cc_im)
fcc.close()

link_im = "//******************************************************************************\n"\
        + "//*NOTE: This file is automatically writen by script, please do not change it  *\n"\
        + "//*If you are an expert, you can ingore the note above                         *\n"\
        + "//******************************************************************************\n\n"\
        + "#ifdef __CINT__\n"\
        + "#pragma link C++ class " + table_name + "+;\n"\
        + "#endif"
flink = open(pathtoclasslink_h, 'w')
flink.write(link_im)
flink.close()

pathtocreatescript = "createtable_" + table_name + ".sql"
cre_im = "#******************************************************************************\n"\
       + "#*NOTE: This file is automatically writen by script, please do not change it  *\n"\
       + "#*If you are an expert, you can ingore the note above                         *\n"\
       + "#******************************************************************************\n\n"\
       + "use " + database_name + ";\n"\
       + "create table " + table_name + " (\n"
dbtype_lis = list()
for index in range(0, len(type_lis)):
 if(type_lis[index] == "long"):
  dbtype_lis.append("BIGINT")
 elif(type_lis[index] == "char*"):
  dbtype_lis.append("VARCHAR(20)")
 else:
  dbtype_lis.append(type_lis[index])

for index in range(0, len(name_lis)-1):
 cre_im = cre_im + name_lis[index] + " " + dbtype_lis[index] + ",\n"
cre_im = cre_im + name_lis[len(name_lis)-1] + " " + dbtype_lis[len(name_lis)-1] + ");"

fcrescr = open(pathtocreatescript, 'w')
fcrescr.write(cre_im)
fcrescr.close
