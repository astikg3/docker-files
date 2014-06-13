#!/usr/bin/python

# Copyright 2013, Gurobi Optimization, Inc.

# This example reads an LP or a MIP from a file, sets a callback
# to monitor the optimization progress, and outputs progress
# information to the screen and to the log file. If the input model
# is a MIP, the callback aborts the optimization after 10000 nodes have
# been explored.


import sys
from gurobipy import *


# Define callback function

def mycallback(model, where):
    if where == GRB.callback.PRESOLVE:
        # Presolve callback
        print('Removed %d columns and %d rows' % \
              (model.cbGet(GRB.callback.PRE_COLDEL), \
               model.cbGet(GRB.callback.PRE_ROWDEL)))
    elif where == GRB.callback.SIMPLEX:
        # Simplex callback
        itcnt = model.cbGet(GRB.callback.SPX_ITRCNT)
        if itcnt - model._lastiter >= 100:
            model._lastiter = itcnt
            obj  = model.cbGet(GRB.callback.SPX_OBJVAL)
            pinf = model.cbGet(GRB.callback.SPX_PRIMINF)
            dinf = model.cbGet(GRB.callback.SPX_DUALINF)
            pert = model.cbGet(GRB.callback.SPX_ISPERT)
            if pert == 0:
                ch = ' '
            elif pert == 1:
                ch = 'S'
            else:
                ch = 'P'
            print('%d %g %s %g %g' % (int(itcnt), obj, ch, pinf, dinf))
    elif where == GRB.callback.MIP:
        # General MIP callback
        nodecnt = model.cbGet(GRB.callback.MIP_NODCNT)
        if nodecnt - model._lastnode >= 100:
            model._lastnode = nodecnt
            objbst = model.cbGet(GRB.callback.MIP_OBJBST)
            objbnd = model.cbGet(GRB.callback.MIP_OBJBND)
            print('%d %g %g' % (int(nodecnt), objbst, objbnd))
        if nodecnt > model._mynodelimit:
            model.terminate()
    elif where == GRB.callback.MIPSOL:
        # MIP solution callback
        obj     = model.cbGet(GRB.callback.MIPSOL_OBJ)
        nodecnt = int(model.cbGet(GRB.callback.MIPSOL_NODCNT))
        print('*** New solution at node %g objective %g' % (nodecnt, obj))
        print(model.cbGetSolution(model.getVars()))
    elif where == GRB.callback.MIPNODE:
        # MIP node callback
        print('*** New node')
        if model.cbGet(GRB.callback.MIPNODE_STATUS) == GRB.status.OPTIMAL:
            x = model.cbGetNodeRel(model.getVars())
            model.cbSetSolution(model.getVars(), x)

if len(sys.argv) < 2:
    print('Usage: callback.py filename')
    quit()

# Read and solve model

model = read(sys.argv[1])

# Pass data into my callback function

model._mynodelimit = 10000
model._lastnode = -1
model._lastiter = -1

model.params.heuristics = 0

model.optimize(mycallback)
