#!/usr/bin/python

# Copyright 2013, Gurobi Optimization, Inc.

# Use parameters that are associated with a model.
#
# A MIP is solved for 5 seconds with different sets of parameters.
# The one with the smallest MIP gap is selected, and the optimization
# is resumed until the optimal solution is found.

import sys
from gurobipy import *

if len(sys.argv) < 2:
    print('Usage: params.py filename')
    quit()


# Simple function to determine the MIP gap
def gap(model):
    if model.solCount == 0 or abs(model.objVal) < 1e-6:
        return GRB.INFINITY
    return abs(model.objBound - model.objVal)/abs(model.objVal)

# Read model and verify that it is a MIP
base = read(sys.argv[1])
if base.isMIP == 0:
    print('The model is not an integer program')
    exit(1)

# Set a 5 second time limit
base.params.timeLimit = 5

# Now solve the model with different values of MIPFocus
bestGap = GRB.INFINITY
bestModel = None
for i in range(4):
    m = base.copy()
    m.params.MIPFocus = i
    m.optimize()
    if bestModel == None or bestGap > gap(m):
        bestModel = m
        bestGap = gap(bestModel)

# Finally, reset the time limit and continue to solve the
# best model to optimality
bestModel.params.timeLimit = "default"
bestModel.optimize()
print('Solved with MIPFocus: %d' % bestModel.params.MIPFocus)
