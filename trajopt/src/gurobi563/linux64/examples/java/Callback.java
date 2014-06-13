/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example reads an LP or a MIP from a file, sets a callback
   to monitor the optimization progress and to output some progress
   information to the screen and to a log file. If it is a MIP and 10%
   gap is reached, then it aborts */

import gurobi.*;

public class Callback extends GRBCallback {
  private GRBVar[] vars;
  private double   lastnode;
  private double   lastiter;

  public Callback(GRBVar[] xvars) {
    vars = xvars;
    lastnode = lastiter = -1;
  }

  protected void callback() {
    try {
      if (where == GRB.CB_MESSAGE) {
        // Message callback
        String st = getStringInfo(GRB.CB_MSG_STRING);
        if (st != null) System.out.print(st);
      } else if (where == GRB.CB_PRESOLVE) {
        // Presolve callback
        int cdels = getIntInfo(GRB.CB_PRE_COLDEL);
        int rdels = getIntInfo(GRB.CB_PRE_ROWDEL);
        System.out.println(cdels+" columns and "+rdels+" rows are removed");
      } else if (where == GRB.CB_SIMPLEX) {
        // Simplex callback
        double itcnt = getDoubleInfo(GRB.CB_SPX_ITRCNT);
        if (itcnt - lastiter >= 100) {
          lastiter = itcnt;
          double obj  = getDoubleInfo(GRB.CB_SPX_OBJVAL);
          double pinf = getDoubleInfo(GRB.CB_SPX_PRIMINF);
          double dinf = getDoubleInfo(GRB.CB_SPX_DUALINF);
          int  ispert = getIntInfo(GRB.CB_SPX_ISPERT);
          char ch;
          if (ispert == 0)      ch = ' ';
          else if (ispert == 1) ch = 'S';
          else                  ch = 'P';
          System.out.println(itcnt+"  "+ obj + ch + "  "+pinf + "  " + dinf);
        }
      } else if (where == GRB.CB_MIP) {
        // General MIP callback
        double nodecnt = getDoubleInfo(GRB.CB_MIP_NODCNT);
        if (nodecnt - lastnode >= 100) {
          lastnode = nodecnt;
          double objbst = getDoubleInfo(GRB.CB_MIP_OBJBST);
          double objbnd = getDoubleInfo(GRB.CB_MIP_OBJBND);
          if (Math.abs(objbst - objbnd) < 0.1 * (1.0 + Math.abs(objbst)))
            abort();
          int actnodes = (int) getDoubleInfo(GRB.CB_MIP_NODLFT);
          int itcnt    = (int) getDoubleInfo(GRB.CB_MIP_ITRCNT);
          int solcnt   = getIntInfo(GRB.CB_MIP_SOLCNT);
          int cutcnt   = getIntInfo(GRB.CB_MIP_CUTCNT);
          System.out.println(nodecnt + " " +  actnodes + " " +  itcnt + " "
            +  objbst + " " +  objbnd + " " +  solcnt + " " +  cutcnt);
        }
      } else if (where == GRB.CB_MIPSOL) {
        // MIP solution callback
        double obj     = getDoubleInfo(GRB.CB_MIPSOL_OBJ);
        int    nodecnt = (int) getDoubleInfo(GRB.CB_MIPSOL_NODCNT);
        double[] x = getSolution(vars);
        System.out.println("**** New solution at node " + nodecnt + ", obj "
                           + obj + ", x[0] = " + x[0] + "****");
      }
    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
          e.getMessage());
      e.printStackTrace();
    }
  }

  public static void main(String[] args) {

    if (args.length < 1) {
      System.out.println("Usage: java Callback filename");
      System.exit(1);
    }

    try {
      GRBEnv    env   = new GRBEnv();
      GRBModel  model = new GRBModel(env, args[0]);

      model.getEnv().set(GRB.IntParam.OutputFlag, 0);

      GRBVar[] vars   = model.getVars();

      // Create a callback object and associate it with the model
      model.setCallback(new Callback(vars));
      model.optimize();

      double[] x      = model.get(GRB.DoubleAttr.X, vars);
      String[] vnames = model.get(GRB.StringAttr.VarName, vars);

      for (int j = 0; j < vars.length; j++) {
        if (x[j] != 0.0) System.out.println(vnames[j] + " " + x[j]);
      }

      // Dispose of model and environment
      model.dispose();
      env.dispose();

    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
          e.getMessage());
      e.printStackTrace();
    }
  }
}
