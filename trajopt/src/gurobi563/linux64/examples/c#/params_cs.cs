/* Copyright 2013, Gurobi Optimization, Inc. */

/* Use parameters that are associated with a model.

   A MIP is solved for 5 seconds with different sets of parameters.
   The one with the smallest MIP gap is selected, and the optimization
   is resumed until the optimal solution is found.
*/

using System;
using Gurobi;

class params_cs
{
  // Simple function to determine the MIP gap
  private static double Gap(GRBModel model) {
    if ((model.Get(GRB.IntAttr.SolCount) == 0) ||
        (Math.Abs(model.Get(GRB.DoubleAttr.ObjVal)) < 1e-6)) {
      return GRB.INFINITY;
    }
    return Math.Abs(model.Get(GRB.DoubleAttr.ObjBound) -
        model.Get(GRB.DoubleAttr.ObjVal)) /
        Math.Abs(model.Get(GRB.DoubleAttr.ObjVal));
  }

  static void Main(string[] args)
  {
    if (args.Length < 1) {
      Console.Out.WriteLine("Usage: params_cs filename");
      return;
    }

    try {
      // Read model and verify that it is a MIP
      GRBEnv env = new GRBEnv();
      GRBModel basemodel = new GRBModel(env, args[0]);
      if (basemodel.Get(GRB.IntAttr.IsMIP) == 0) {
        Console.WriteLine("The model is not an integer program");
        Environment.Exit(1);
      }

      // Set a 5 second time limit
      basemodel.GetEnv().Set(GRB.DoubleParam.TimeLimit, 5);

      // Now solve the model with different values of MIPFocus
      double bestGap = GRB.INFINITY;
      GRBModel bestModel = null;
      for (int i = 0; i <= 3; ++i) {
        GRBModel m = new GRBModel(basemodel);
        m.GetEnv().Set(GRB.IntParam.MIPFocus, i);
        m.Optimize();
        if (bestModel == null || bestGap > Gap(m)) {
          bestModel = m;
          bestGap = Gap(bestModel);
        }
      }

      // Finally, reset the time limit and continue to solve the
      // best model to optimality
      bestModel.GetEnv().Set(GRB.DoubleParam.TimeLimit, GRB.INFINITY);
      bestModel.Optimize();
      Console.WriteLine("Solved with MIPFocus: " +
          bestModel.GetEnv().Get(GRB.IntParam.MIPFocus));

    } catch (GRBException e) {
      Console.WriteLine("Error code: " + e.ErrorCode + ". " +
          e.Message);
    }
  }
}
