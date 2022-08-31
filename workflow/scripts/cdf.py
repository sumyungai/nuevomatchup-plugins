# -*- coding: utf-8 -*-
"""
Created on Mon Aug 15 16:41:33 2022

@author: ruste
"""
from collections import defaultdict
from decimal import Decimal
import matplotlib.pyplot as plt

if 'snakemake' not in globals():
    class Snakemake:
        def _init_(self, input, output):
            self.input = input
            self.output = output
    snakemake = Snakemake([ "map_file.txt" ], { 'txt': "test.txt", 'png': "test.png" })

input_files = snakemake.input
output_png = snakemake.output['png']
output_txt = snakemake.output['txt']

prec = Decimal('0.01')

sums = defaultdict(int)
for fname in input_files:
    with open(fname) as fd:
        sum_y = 0
        for line in fd:
            x, y = line.split()
            x = Decimal(x).quantize(prec)
            sum_y += float(y)
            sums[x] += sum_y
list_x = list(sums.keys())
list_s_raw = list(sums.values())

list_x.sort()
list_s_raw.sort()

list_s = [ s / list_s_raw[-1] for s in list_s_raw ]

with open(output_txt, 'w') as fd:
    for i in range(len(list_x)):
        fd.write(str(list_x[i]) + "\t" + str(list_s[i]))
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.set_title("CDF")
ax1.set_xlabel("Data")
ax1.set_ylabel("Dist")
ax1.plot(list_x, list_s, c = 'r', label = output_txt, linewidth = 2)
ax1.legend()

plt.savefig(output_png)
