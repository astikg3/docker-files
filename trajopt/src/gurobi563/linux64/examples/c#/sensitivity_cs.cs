/* Copyright 2013, Gurobi Optimization, Inc. */

/* Simple MIP sensitivity analysis example.
   For each integer variable, fix it to its lower and upper bound
   and check the impact on the objective. */

using System;
using Gurobi;

class sensitivity_cs
{
  static void Main(string[] args)
  {
    if (args.Length < 1) {
      Console.Out.WriteLine("Usage: sensitivity_cs filename");
      return;
    }

    try {
      // Read model
      GRBEnv env = new GRBEnv();
      GRBModel a = new GRBModel(env, args[0]);
      a.Optimize();
      a.GetEnv().Set(GRB.IntParam.OutputFlag, 0);

      // Extract variables from model
      GRBVar[] avars = a.GetVars();

      for (int i = 0; i < avars.Length; ++i) {
        GRBVar v = avars[i];
        if (v.Get(GRB.CharAttr.VType) == GRB.BINARY) {

          // Create clone and fix variable
          GRBModel b = new GRBModel(a);
          GRBVar bv = b.GetVars()[i];
          if (v.Get(GRB.DoubleAttr.X) - v.Get(GRB.DoubleAttr.LB) < 0.5) {
            bv.Set(GRB.DoubleAttr.LB, bv.Get(GRB.DoubleAttr.UB));
          } else {
            bv.Set(GRB.DoubleAttr.UB, bv.Get(GRB.DoubleAttr.LB));
          }

          b.Optimize();

          if (b.Get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
            double objchg =
                b.Get(GRB.DoubleAttr.ObjVal) - a.Get(GRB.DoubleAttr.ObjVal);
            if (objchg < 0) {
              objchg = 0;
            }
            Console.WriteLine("Objective sensitivity for variable " +
                v.Get(GRB.StringAttr.VarName) + " is " + objchg);
          } else {
            Console.WriteLine("Objective sensitivity for variable " +
                v.Get(GRB.StringAttr.VarName) + " is infinite");
          }

          // Dispose of model
          b.Dispose();
        }
      }

      // Dispose of model and env
      a.Dispose();
      env.Dispose();

    } catch (GRBException e) {
      Console.WriteLine("Error code: " + e.ErrorCode + ". " +
          e.Message);
    }
  }
}
