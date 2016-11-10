#!/usr/bin/python

import sys
import math
import numpy as np
from gurobipy import *

if len(sys.argv) < 5:
    print('Usage: script.py lpfilename namein nameout datafilename deviation_percentage')
    exit(0)

log = open("mygurobi.log","w")

# Read and solve model
print "Enter a deviation percentage for risk estimation: a number > 0 & < 1"
per = float(raw_input())


model = read(sys.argv[1])
model.optimize()

log.write('Solving %s\n' % sys.argv[1])
log.write("variables = " + str(model.NumVars) + "\n")
log.write("constraints = " + str(model.NumConstrs) + "\n")

if model.status == GRB.status.INF_OR_UNBD:
    print "->LP infeasible or unbounded"
    log.write('->LP infeasible or unbounded\n')
    log.close()
    exit(0)

log.write('Optimal objective = %g\n' % model.objVal)

count = 0

# save the portfolio with x[]

x = []

for v in model.getVars():
    if v.varname[0] == 'x':
        x.append(v.x)
    if math.fabs(v.x) > 0.0000001:
        count += 1

####print x
log.write(str(count) + " nonzero variables in solution\n")
for v in model.getVars():
    if math.fabs(v.x) > 0.0000001:
        print( v.varname + " = " +str(v.x))
        log.write( v.varname + " = " +str(v.x) + "\n")

log.write("bye.\n")
log.close()

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass
    return False

# save the deviation used previously

devi = float(sys.argv[5])

# read the datafile as data
with open(sys.argv[4]) as f:
    data = f.readlines()
data = [i.strip().split() for i in data]
data = [i for i in data if i]

# extract the prices
count = 0
n_sec = int(data[0][1])+1 # security 0 as cash
n_sce = int(data[0][3])+1 # scene 0 as today
r = float(data[0][5])
price = [[None]*n_sec]*(n_sce)

for i in range(1,len(data)):
    if is_number(data[i][0]):
        price[count] = data[i-1] + data[i]
        price[count][0] = 1 if count == 0 else 1 + r
        count = count + 1
price = [[float(j) for j in i] for i in price]

####print price


# simulate the security prices and calculate the loss

def getRisk(sigma=per):
    loss_all = []
    ### loop through each scenario i
    for i in range(1,n_sce):
        loss_i = 0
        ### in scenario i, loop through each security sec
        for sec in range(n_sec):
            loss_i = loss_i + (price[0][sec] - price[i][sec]*np.random.normal(1,sigma))*x[sec]
        loss_all.append(max(0,loss_i))
    risk = max(loss_all)
    return (risk)
result = [getRisk() for _ in range(100)]

# write the result to file
riskfile = open('risk.log','w')

riskfile.write("The risk of 100 trials: \n\n")
riskfile.write(str(result) + "\n")
riskfile.write("\nThe statistics: \n")
riskfile.write("\nMean: {:f}, Std: {:f}, Max: {:f}, Min: {:f}.".format(np.mean(result),np.std(result), max(result), min(result)))

riskfile.write("\n\nbye.\n")
riskfile.close()

print "renaming ", sys.argv[2], "into ", sys.argv[3]
os.rename(sys.argv[2], sys.argv[3])
