/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example reads an LP or a MIP from a file, sets a callback
   to monitor the optimization progress and to output some progress
   information to the screen and to a log file. If it is a MIP and 10%
   gap is reached, then it aborts */

#include "gurobi_c++.h"
#include <cmath>
using namespace std;

class mycallback: public GRBCallback
{
  public:
    GRBVar* vars;
    int numvars;
    double lastnode;
    double lastiter;
    mycallback(GRBVar* xvars, int xnumvars) {
      vars = xvars;
      numvars = xnumvars;
      lastnode = lastiter = -1;
    }
  protected:
    void callback () {
      try {
        if (where == GRB_CB_MESSAGE) {
          // Message callback
          string msg = getStringInfo(GRB_CB_MSG_STRING);
          cout << msg;
        } else if (where == GRB_CB_PRESOLVE) {
          // Presolve callback
          int cdels = getIntInfo(GRB_CB_PRE_COLDEL);
          int rdels = getIntInfo(GRB_CB_PRE_ROWDEL);
          cout << cdels << " columns and " << rdels
               << " rows are removed" << endl;
        } else if (where == GRB_CB_SIMPLEX) {
          // Simplex callback
          double itcnt = getDoubleInfo(GRB_CB_SPX_ITRCNT);
          if (itcnt - lastiter >= 100) {
            lastiter = itcnt;
            double obj  = getDoubleInfo(GRB_CB_SPX_OBJVAL);
            double pinf = getDoubleInfo(GRB_CB_SPX_PRIMINF);
            double dinf = getDoubleInfo(GRB_CB_SPX_DUALINF);
            int  ispert = getIntInfo(GRB_CB_SPX_ISPERT);
            char ch;
            if (ispert == 0)      ch = ' ';
            else if (ispert == 1) ch = 'S';
            else                  ch = 'P';
            cout << itcnt << "  " << obj << ch << "  " << pinf
                 << "  " << dinf << endl;
          }
        } else if (where == GRB_CB_MIP) {
          // General MIP callback
          double nodecnt = getDoubleInfo(GRB_CB_MIP_NODCNT);
          if (nodecnt - lastnode >= 100) {
            lastnode = nodecnt;
            double objbst = getDoubleInfo(GRB_CB_MIP_OBJBST);
            double objbnd = getDoubleInfo(GRB_CB_MIP_OBJBND);
            if (fabs(objbst - objbnd) < 0.1 * (1.0 + fabs(objbst)))
              abort();
            int actnodes = (int) getDoubleInfo(GRB_CB_MIP_NODLFT);
            int itcnt    = (int) getDoubleInfo(GRB_CB_MIP_ITRCNT);
            int solcnt   = getIntInfo(GRB_CB_MIP_SOLCNT);
            int cutcnt   = getIntInfo(GRB_CB_MIP_CUTCNT);
            cout << nodecnt << " " <<  actnodes << " " <<  itcnt << " "
                 << objbst << " " <<  objbnd << " "  <<  solcnt << " "
                 <<  cutcnt << endl;
          }
        } else if (where == GRB_CB_MIPSOL) {
          // MIP solution callback
          double obj     = getDoubleInfo(GRB_CB_MIPSOL_OBJ);
          int    nodecnt = (int) getDoubleInfo(GRB_CB_MIPSOL_NODCNT);
          double* x = getSolution(vars, numvars);
          cout << "**** New solution at node " << nodecnt << ", obj "
                             << obj << ", x[0] = " << x[0] << "****" << endl;
          delete[] x;
        }
      } catch (GRBException e) {
        cout << "Error number: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
      } catch (...) {
        cout << "Error during callback" << endl;
      }
    }
};

int
main(int   argc,
     char *argv[])
{
  if (argc < 2) {
    cout << "Usage: callback_c++ filename" << endl;
    return 1;
  }

  GRBEnv *env = 0;
  GRBVar *vars = 0;
  try {
    env = new GRBEnv();
    GRBModel model = GRBModel(*env, argv[1]);

    model.getEnv().set(GRB_IntParam_OutputFlag, 0);

    int numvars = model.get(GRB_IntAttr_NumVars);

    vars = model.getVars();

    // Create a callback object and associate it with the model
    mycallback cb = mycallback(vars, numvars);
    model.setCallback(&cb);

    model.optimize();

    for (int j = 0; j < numvars; j++) {
      GRBVar v = vars[j];
      cout << v.get(GRB_StringAttr_VarName) << " "
           << v.get(GRB_DoubleAttr_X) << endl;
    }
  } catch (GRBException e) {
    cout << "Error number: " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch (...) {
    cout << "Error during optimization" << endl;
  }

  delete[] vars;
  delete env;
  return 0;
}
