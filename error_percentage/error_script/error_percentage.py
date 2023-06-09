# -*- coding: utf-8 -*-
"""
Created on Sun Jan  8 17:07:43 2023

@author: ruste
"""

from colour import Color
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from numpy import mean, std
import math
import os
import re

def geom(a):
    avg = 1
    for i in range(len(a)):
        avg = avg*a[i]
    return avg**(1/len(a))
    
rq_rmi_re = re.compile('RQ-RMI error list: \[(.+)\]')
size_re = re.compile('Training RQRMI of size (\d+),(\d+),(\d+)')
ranges_re = re.compile('ranges: (\d+)')
submodels = 0, 1, -1
num_submodels = len(submodels)
dirfloating = 'D:\Errorfiles\\floatingPoint\\'
dirs = os.listdir(dirfloating)
num_submodels = len(submodels)
ever = {}
num_prec = len(dirs)
for d in dirs:
    temp_er = []
    ranges = []
    sumlast = []
    for file in os.listdir(os.path.join(dirfloating,d)):
        sizes = []
        errors = []
        with open(os.path.join(dirfloating,d,file)) as fd:
            for line in fd:
                if search := ranges_re.search(line):
                    ranges.append(int(search.group(1)))
                if search := size_re.search(line):
                    sizes += [ int(s) for s in search.groups() ]
                if search := rq_rmi_re.search(line):
                   errors.append([int(e) for e in search.group(1).split(',')])
            sumlast.append(max(errors[-1]))
            
    
    print(d + ", " + str(sum(sumlast)/10))
    temp_er.append([ errors[0][0]/ 688884 *100 ])
    for i in range(1, sizes[1]):
        if errors[1][i]!=-1:
            temp_er.append( [errors[1][i]/ranges[i]*100] )
    
    for i in range(1, sizes[2]):
        if errors[-1][-i]!=-1:
            temp_er.append( [errors[-1][-i]/ranges[-i]*100] )   

  
    ever[d] = [geom(temp_er[2])]
    print (d + ", " + str(ever[d]))


colour1 = Color("midnightblue")

keys = ['float8_1', 'float8_2', 'Bfloat16', 'half_prec', 'fp32', 'org']
evver = {i: ever[i] for i in keys}

new_keys = ['6 exp, 1 man', '5 exp, 2 man', '8 exp, 7 man', '5 exp, 10 man', '8 exp, 24 man', 'org']

space = 0.2
offset = (1 - space) / num_submodels

fig1 = plt.figure()
fig1.set_size_inches(10, 8)

ax1 = fig1.add_subplot(111)
ax1.set_xlabel("Format", fontsize=13)
ax1.set_ylabel("Error percentage", fontsize=13)

shift = (num_submodels - 1) / 2
for file_ix, (file, errors) in enumerate(evver.items()):

    ax1.bar(file, errors, color = 'red', width = offset, linewidth = 2)

labels = [ f.strip(".txt") for f in dirs]
ax1.set_xticks(range(num_prec), new_keys)
ax1.set_yscale('log')
