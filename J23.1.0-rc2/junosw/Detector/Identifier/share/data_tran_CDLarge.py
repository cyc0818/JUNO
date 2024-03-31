# this is a code, used to read all_pmt_xxxxxx.xlsx documment, and transpate to .csv
#this file need to run in conda environment, for need using some python packages
# author wuchx 2022 10 08
import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
import seaborn as sns  # for making plots with seaborn
import math
import csv
import re

in_read_file="./all_pmt_20230326.xlsx"
out_file = "all_pmt_20230326_CDLPMT.csv"
in_read_GCU_file="./all_pmt_20230326_GCU.xlsx"

file1 = open(out_file, 'w', encoding='utf-8')
csv_writer = csv.writer(file1, delimiter=' ' ) 
#csv_writer = csv.writer(file1 )

##-------choose sheet ------sheet1 CD PMT, sheet2 sPMT, sheet3 Veto PMT, sheet4 sPMT-UWB-V15
sheet_name='CD'   # test CD PMT
data = pd.read_excel(in_read_file, sheet_name=sheet_name) 
data_GCU = pd.read_excel(in_read_GCU_file, sheet_name="Sheet1") 

inital = 0 #read data inital row
end= inital + 17612 #read ddata end row ----- CD_PMT number 17612 
index = 0

for i in range(inital, end):  ## end is not include
    ID = data.iloc[i,1]
    CDNumber = data.iloc[i,2]
    LargeOrSmall = 0 
    NorthOrSouth = -1 #set -1
    if data.iloc[i,3] == 'N' or  data.iloc[i,3] == 'n':   #16290 is little “s”  
        NorthOrSouth = 0
    if data.iloc[i,3] == 'S' or data.iloc[i,3] == 's' :   #16290 is little “s” 
        NorthOrSouth = 1
    
    CircleNumber=data.iloc[i,4]
    LineNumber=data.iloc[i,5]
    x = data.iloc[i,6]

    UpOrDown = 0

    PMTType = -1
    if data.iloc[i,9] == 'H' or  data.iloc[i,9] == 'h':    
        PMTType = 1
    if data.iloc[i,9] == 'N' or data.iloc[i,9] == 'n' :   
        PMTType = 2


    GCU = np.nan_to_num(data_GCU.iloc[i,10]) 
    if GCU !=0:
        GCU = re.sub('\D','', GCU)
    # else:
        #GCU = re.sub('\D','', GCU)

    channel = np.nan_to_num(data_GCU.iloc[i,11])
    if channel != 0:
        channel = re.findall(r"[(](.*?)[)]", channel)
        channel = channel[0]
    # else:
        #channel = re.findall(r"[(](.*?)[)]", channel)

    #GCU = re.sub('\D','',data_GCU.iloc[i,10])
    #GCU = ''.join( [x for x in data_GCU.iloc[i,10] if x.isdigit()])
    #channel = re.findall(r"[(](.*?)[)]", data_GCU.iloc[i,11])
    


    index=index+1

    csv_writer.writerow([ID, CDNumber, LargeOrSmall, NorthOrSouth, CircleNumber, LineNumber, UpOrDown, PMTType, GCU, channel])
    


    if i == inital or  i == (end-1):  ## end is not include
        print("ID=%d   CDNumber=%s  NorthOrSouth=%d  CircleNumber=%d  LineNumber=%d X= %f\n"  %(ID, CDNumber, NorthOrSouth, CircleNumber, LineNumber,x))

file1.close()

print("read and write %d rows data" %index)

# print(data.head()) 

