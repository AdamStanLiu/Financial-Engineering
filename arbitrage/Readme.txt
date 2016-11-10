Usage:

Open the command window (with python 2, numpy and gurobi installed):

1. change the direction to this folder;

2. type: "arbrob.exe datafile_name(default: sample.dat) lpfile_name(default: sample.lp) deviation_percentage(i.e. 0.02)"

3. when the cmd shows the prompt: "Enter a deviation percentage for risk estimation: a number > 0 & < 1", 
   please input the percentage you want to use for risk calculation, and then press "Enter"

Note: If you enter a negative number or any other type of input rather than a number, i.e a string or charactor, it would cause error. Please exit and restart the program.
      If you enter a number bigger than one, it may still offer you a result but may be meaningless.

Best