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

in_read_file="./all_pmt_20230326.xlsx"
out_file = "all_pmt_20230326_CDSPMT.csv"

file1 = open(out_file, 'w', encoding='utf-8')
csv_writer = csv.writer(file1, delimiter=' ' ) 
#csv_writer = csv.writer(file1 )

##-------choose sheet ------sheet1 CD PMT, sheet2 sPMT, sheet3 Veto PMT, sheet4 sPMT-UWB-V15
sheet_name='sPMT'   # test CD PMT
data = pd.read_excel(in_read_file, sheet_name=sheet_name) 

inital = 0 #read data inital row
end= inital + 25600 #read ddata end row ----- CD_PMT number 17612 
index = 0

for i in range(inital, end):  ## end is not include
    ID = data.iloc[i,1]-20000+17612  #
    CDNumber = data.iloc[i,11]
    LargeOrSmall = 1 
    NorthOrSouth = -1 #set -1
    if data.iloc[i,12] == 'N' or  data.iloc[i,12] == 'n':   #16290 is little “s”  
        NorthOrSouth = 0
    if data.iloc[i,12] == 'S' or data.iloc[i,12] == 's' :   #16290 is little “s” 
        NorthOrSouth = 1
    
    CircleNumber=data.iloc[i,13]
    LineNumber=data.iloc[i,14]
    x = data.iloc[i,4]

    UpOrDown = -1
    if data.iloc[i,15] == 'U' or  data.iloc[i,15] == 'u':    
        UpOrDown = 0
    if data.iloc[i,15] == 'D' or data.iloc[i,15] == 'd' :   
        UpOrDown = 1

    PMTType = 3
    # if data.iloc[i,7] == 'H' or  data.iloc[i,1] == 'h':    
    #     PMTType = 1
    # if data.iloc[i,7] == 'N' or data.iloc[i,1] == 'n' :   
    #     PMTType = 2


    index=index+1

    csv_writer.writerow([ID, CDNumber, LargeOrSmall, NorthOrSouth, CircleNumber, LineNumber, UpOrDown, PMTType])
    


    if i == inital or  i == (end-1):  ## end is not include
        print("ID=%d   CDNumber=%s  NorthOrSouth=%d  CircleNumber=%d  LineNumber=%d X= %f\n"  %(ID, CDNumber, NorthOrSouth, CircleNumber, LineNumber,x))

file1.close()

print("read and write %d rows data" %index)

# print(data.head()) 

