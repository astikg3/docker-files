/* Copyright 2013, Gurobi Optimization, Inc. */

/* Use parameters that are associated with a model.

   A MIP is solved for 5 seconds with different sets of parameters.
   The one with the smallest MIP gap is selected, and the optimization
   is resumed until the optimal solution is found.
*/

import gurobi.*;

public class Params {

  // Simple function to determine the MIP gap
  private static double gap(GRBModel model) throws GRBException {
    if ((model.get(GRB.IntAttr.SolCount) == 0) ||
        (Math.abs(model.get(GRB.DoubleAttr.ObjVal)) < 1e-6)) {
      return GRB.INFINITY;
    }
    return Math.abs(model.get(GRB.DoubleAttr.ObjBound) -
        model.get(GRB.DoubleAttr.ObjVal)) /
        Math.abs(model.get(GRB.DoubleAttr.ObjVal));
  }

  public static void main(String[] args) {

    if (args.length < 1) {
      System.out.println("Usage: java Params filename");
      System.exit(1);
    }

    try {
      // Read model and verify that it is a MIP
      GRBEnv env = new GRBEnv();
      GRBModel base = new GRBModel(env, args[0]);
      if (base.get(GRB.IntAttr.IsMIP) == 0) {
        System.out.println("The model is not an integer program");
        System.exit(1);
      }

      // Set a 5 second time limit
      base.getEnv().set(GRB.DoubleParam.TimeLimit, 5);

      // Now solve the model with different values of MIPFocus
      double bestGap = GRB.INFINITY;
      GRBModel bestModel = null;
      for (int i = 0; i <= 3; ++i) {
        GRBModel m = new GRBModel(base);
        m.getEnv().set(GRB.IntParam.MIPFocus, i);
        m.optimize();
        if (bestModel == null || bestGap > gap(m)) {
          bestModel = m;
          bestGap = gap(bestModel);
        }
      }

      // Finally, reset the time limit and continue to solve the
      // best model to optimality
      bestModel.getEnv().set(GRB.DoubleParam.TimeLimit, GRB.INFINITY);
      bestModel.optimize();
      System.out.println("Solved with MIPFocus: " +
          bestModel.getEnv().get(GRB.IntParam.MIPFocus));

    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
          e.getMessage());
    }
  }
}
