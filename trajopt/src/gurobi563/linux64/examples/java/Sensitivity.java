/* Copyright 2013, Gurobi Optimization, Inc. */

/* Simple MIP sensitivity analysis example.
 For each integer variable, fix it to its lower and upper bound
 and check the impact on the objective. */

import gurobi.*;

public class Sensitivity {

  public static void main(String[] args) {

    if (args.length < 1) {
      System.out.println("Usage: java Sensitivity filename");
      System.exit(1);
    }

    try {
      // Read model
      GRBEnv env = new GRBEnv();
      GRBModel a = new GRBModel(env, args[0]);
      a.optimize();
      a.getEnv().set(GRB.IntParam.OutputFlag, 0);

      // Extract variables from model
      GRBVar[] avars = a.getVars();

      for (int i = 0; i < avars.length; ++i) {
        GRBVar v = avars[i];
        if (v.get(GRB.CharAttr.VType) == GRB.BINARY) {

          // Create clone and fix variable
          GRBModel b = new GRBModel(a);
          GRBVar bv = b.getVars()[i];
          if (v.get(GRB.DoubleAttr.X) - v.get(GRB.DoubleAttr.LB) < 0.5) {
            bv.set(GRB.DoubleAttr.LB, bv.get(GRB.DoubleAttr.UB));
          } else {
            bv.set(GRB.DoubleAttr.UB, bv.get(GRB.DoubleAttr.LB));
          }

          b.optimize();

          if (b.get(GRB.IntAttr.Status) == GRB.Status.OPTIMAL) {
            double objchg =
                b.get(GRB.DoubleAttr.ObjVal) - a.get(GRB.DoubleAttr.ObjVal);
            if (objchg < 0) {
              objchg = 0;
            }
            System.out.println("Objective sensitivity for variable " +
                v.get(GRB.StringAttr.VarName) + " is " + objchg);
          } else {
            System.out.println("Objective sensitivity for variable " +
                v.get(GRB.StringAttr.VarName) + " is infinite");
          }

          // Dispose of model
          b.dispose();
        }
      }

      // Dispose of model and environment
      a.dispose();
      env.dispose();

    } catch (GRBException e) {
      System.out.println("Error code: " + e.getErrorCode() + ". " +
          e.getMessage());
    }
  }
}
