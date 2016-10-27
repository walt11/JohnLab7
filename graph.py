''' 
Name: John Walter
Date: 10/22/16

This script was written for python 2.7.6 (default installed in linux)
In order to use this script, matplotlib must be installed. 
To install on linux run:
sudo apt-get install python-matplotlib

'''

import numpy as np
import matplotlib.pyplot as plt

raw = []
raw_data = []
new_data=[]
i = 0;

# read in the file containing the names of the files with contents to be loaded
with open("names.txt") as f:
	names = f.readlines()
# read in the contents of the file that has the first name within the names.txt file
with open(names[0].rstrip()) as f:
	content = f.readlines()

# remove the newline characters
flag = names[2].rstrip('\n');

# read in the second file
for item in content:
	raw_data.append(item.rstrip('\n'))

raw_data.pop(0)

# this is for the name of the graph
if(int(flag) == 1):
	alter = "Offset"
elif(int(flag) == 2):
	alter = "Scaled"
elif(int(flag) == 3):
	alter = "Centered"
else:
	alter = "Normalized"

# read in the second file	
with open(names[1].rstrip()) as y:
	new = y.readlines()
for item in new:
	new_data.append(item.rstrip('\n'))
new_data.pop(0)

# plot the data from the two files
plt.title(alter)
plt.plot(raw_data,"r")
plt.plot(new_data,"g")
plt.axis()
plt.show()
