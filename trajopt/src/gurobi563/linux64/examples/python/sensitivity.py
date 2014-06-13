#!/usr/bin/python

# Copyright 2013, Gurobi Optimization, Inc.

# Simple MIP sensitivity analysis example.
# For each integer variable, fix it to its lower and upper bound
# and check the impact on the objective.

import sys
from gurobipy import *

if len(sys.argv) < 2:
    print('Usage: sensitivity.py filename')
    quit()


a = gurobi.read(sys.argv[1])
a.optimize()
a.params.outputFlag = 0

# Extract variables from model

avars = a.getVars()

# Iterate through binary variables in model

for i in range(len(avars)):
    v = avars[i]
    if v.vType == GRB.BINARY:

# Create clone and fix variable

        b = a.copy()
        bv = b.getVars()[i]
        if v.x - v.lb < 0.5:
            bv.lb = bv.ub
        else:
            bv.ub = bv.lb

        b.optimize()

        if b.status == GRB.status.OPTIMAL:
            objchg = b.objVal - a.objVal
            if objchg < 0:
                objchg = 0
            print('Objective sensitivity for variable %s is %g' % \
                  (v.varName, objchg))
        else:
            print('Objective sensitivity for variable %s is infinite' % \
                  v.varName)
