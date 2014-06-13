/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example reads an LP or a MIP from a file, sets a callback
   to monitor the optimization progress and to output some progress
   information to the screen and to a log file. If it is a MIP and 10%
   gap is reached, then it aborts. */

using System;
using Gurobi;

class callback_cs : GRBCallback
{
  private GRBVar[] vars;
  private double   lastnode;
  private double   lastiter;

  public callback_cs(GRBVar[] xvars)
  {
    vars = xvars;
    lastnode = lastiter = -1;
  }

  protected override void Callback()
  {
    try {
      if (where == GRB.Callback.MESSAGE) {
        // Message callback
        string st = GetStringInfo(GRB.Callback.MSG_STRING);
        if (st != null) Console.Write(st);
      } else if (where == GRB.Callback.PRESOLVE) {
        // Presolve callback
        int cdels = GetIntInfo(GRB.Callback.PRE_COLDEL);
        int rdels = GetIntInfo(GRB.Callback.PRE_ROWDEL);
        Console.WriteLine(cdels+" columns and "+rdels+" rows are removed");
      } else if (where == GRB.Callback.SIMPLEX) {
        // Simplex callback
        double itcnt = GetDoubleInfo(GRB.Callback.SPX_ITRCNT);
        if (itcnt - lastiter >= 100) {
          lastiter = itcnt;
          double obj  = GetDoubleInfo(GRB.Callback.SPX_OBJVAL);
          double pinf = GetDoubleInfo(GRB.Callback.SPX_PRIMINF);
          double dinf = GetDoubleInfo(GRB.Callback.SPX_DUALINF);
          int  ispert = GetIntInfo(GRB.Callback.SPX_ISPERT);
          char ch;
          if (ispert == 0)      ch = ' ';
          else if (ispert == 1) ch = 'S';
          else                  ch = 'P';
          Console.WriteLine(itcnt+"  "+ obj + ch + "  "+pinf + "  " + dinf);
        }
      } else if (where == GRB.Callback.MIP) {
        // General MIP callback
        double nodecnt = GetDoubleInfo(GRB.Callback.MIP_NODCNT);
        if (nodecnt - lastnode >= 100) {
          lastnode = nodecnt;
          double objbst = GetDoubleInfo(GRB.Callback.MIP_OBJBST);
          double objbnd = GetDoubleInfo(GRB.Callback.MIP_OBJBND);
          if (Math.Abs(objbst - objbnd) < 0.1 * (1.0 + Math.Abs(objbst)))
            Abort();
          int actnodes = (int) GetDoubleInfo(GRB.Callback.MIP_NODLFT);
          int itcnt    = (int) GetDoubleInfo(GRB.Callback.MIP_ITRCNT);
          int solcnt   = GetIntInfo(GRB.Callback.MIP_SOLCNT);
          int cutcnt   = GetIntInfo(GRB.Callback.MIP_CUTCNT);
          Console.WriteLine(nodecnt + " " +  actnodes + " " +  itcnt + " "
            +  objbst + " " +  objbnd + " " +  solcnt + " " +  cutcnt);
        }
      } else if (where == GRB.Callback.MIPSOL) {
        // MIP solution callback
        double obj     = GetDoubleInfo(GRB.Callback.MIPSOL_OBJ);
        int    nodecnt = (int) GetDoubleInfo(GRB.Callback.MIPSOL_NODCNT);
        double[] x = GetSolution(vars);
        Console.WriteLine("**** New solution at node " + nodecnt + ", obj "
                           + obj + ", x[0] = " + x[0] + "****");
      }
    } catch (GRBException e) {
      Console.WriteLine("Error code: " + e.ErrorCode + ". " + e.Message);
      Console.WriteLine(e.StackTrace);
    }
  }

  static void Main(string[] args)
  {
    if (args.Length < 1) {
      Console.Out.WriteLine("Usage: callback_cs filename");
      return;
    }

    try {
      GRBEnv    env   = new GRBEnv();
      GRBModel  model = new GRBModel(env, args[0]);

      GRBVar[] vars   = model.GetVars();

      // Create a callback object and associate it with the model
      model.SetCallback(new callback_cs(vars));
      model.Optimize();

      double[] x      = model.Get(GRB.DoubleAttr.X, vars);
      string[] vnames = model.Get(GRB.StringAttr.VarName, vars);

      for (int j = 0; j < vars.Length; j++) {
        if (x[j] != 0.0) Console.WriteLine(vnames[j] + " " + x[j]);
      }

      // Dispose of model and env
      model.Dispose();
      env.Dispose();

    } catch (GRBException e) {
      Console.WriteLine("Error code: " + e.ErrorCode + ". " + e.Message);
      Console.WriteLine(e.StackTrace);
    }
  }
}
