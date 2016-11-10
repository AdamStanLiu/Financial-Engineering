# -*- coding: utf-8 -*-
"""
Created on Sun Sep 18 19:33:44 2016

@author: Zidong Liu
"""

import sys
import numpy as np
from yahoo_finance import Share
from pprint import pprint

# Read the stock symbols.

filename = 'russell_1000_ticker.txt'

try:
    f = open(filename, 'r') # opens the input file
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

lines = f.readlines();

result = open ('hw1v1_result.txt','w')

# Initialize the variables

count = 1
prices = {}
return_d = {}
return_mean = {}
return_va = {}
autocorr_1d= {}
autocorr_5d= {}
autocorr_10d= {}

# create a function to compute autocorrelation

def autocorrelation(array, d):
    me = np.mean(array)
    va = np.var(array)
    array[:] = [x - me for x in array]
    array1 = np.array(array[:-d])
    array2 = np.array(array[d:])
    autocorr = np.mean(array1 * array2) / va
    return autocorr

# Extract the closing price from yahoo finance

for line in lines:
    thisline = line.split()
    if len(line) > 0:
        
        try:
            share = Share(thisline[0])
            everything = share.get_historical('2015-01-01', '2015-07-01')
        except Exception:
            pass
        
        prices[thisline[0]] = [0 for j in xrange(len(everything))]
        return_d[thisline[0]] = [0 for j in xrange(len(everything)-1)]
        
        for j in xrange(len(everything)):
            prices[thisline[0]][j] = everything[j]['Adj_Close']
            # calculate the return
            if j < len(everything) - 1:
                return_d[thisline[0]][j] = (float(everything[j+1]['Adj_Close']) - float(everything[j]['Adj_Close'])) / float(everything[j]['Adj_Close'])
        
        # calculate the mean, variance and autocorrelations        
        return_mean[thisline[0]] = np.mean(return_d[thisline[0]])
        return_va[thisline[0]] = np.var(return_d[thisline[0]])
        autocorr_1d[thisline[0]] = autocorrelation(return_d[thisline[0]], 1)
        autocorr_5d[thisline[0]] = autocorrelation(return_d[thisline[0]], 5)
        autocorr_10d[thisline[0]] = autocorrelation(return_d[thisline[0]], 10)
        
        # print
        print thisline[0] + ": mean: " + str(return_mean[thisline[0]]) + ", variance: " + str(return_va[thisline[0]])
        print "autocorrelation(1d): " + str(autocorr_1d[thisline[0]])
        print "autocorrelation(5d): " + str(autocorr_5d[thisline[0]])
        print "autocorrelation(10d): " + str(autocorr_10d[thisline[0]]) + "\n"
        
        # write the result to a txt file
        result.write(thisline[0] + ": mean: " + str(return_mean[thisline[0]]) + ", variance: " + str(return_va[thisline[0]]) + "\n"
                    + "autocorrelation(1d): " + str(autocorr_1d[thisline[0]]) + "\n"
                    + "autocorrelation(5d): " + str(autocorr_5d[thisline[0]]) + "\n"
                    + "autocorrelation(10d): " + str(autocorr_10d[thisline[0]]) + "\n" + "\n")
        
        count += 1
    '''    
    if count == 5:
        break
    '''
        
# close the file          

f.close()
result.close()



