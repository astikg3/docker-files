/* Copyright 2013, Gurobi Optimization, Inc. */

/* Assign workers to shifts; each worker may or may not be available on a
   particular day. If the problem cannot be solved, add slack variables
   to determine which constraints cannot be satisfied, and how much
   they need to be relaxed. */

using System;
using System.Collections.Generic;
using Gurobi;

class workforce3_cs
{
  static void Main()
  {
    try {

      // Sample data
      // Sets of days and workers
      string[] Shifts =
          new string[] { "Mon1", "Tue2", "Wed3", "Thu4", "Fri5", "Sat6",
              "Sun7", "Mon8", "Tue9", "Wed10", "Thu11", "Fri12", "Sat13",
              "Sun14" };
      string[] Workers =
          new string[] { "Amy", "Bob", "Cathy", "Dan", "Ed", "Fred", "Gu" };

      int nShifts = Shifts.Length;
      int nWorkers = Workers.Length;

      // Number of workers required for each shift
      double[] shiftRequirements =
          new double[] { 3, 2, 4, 4, 5, 6, 5, 2, 2, 3, 4, 6, 7, 5 };

      // Amount each worker is paid to work one shift
      double[] pay = new double[] { 10, 12, 10, 8, 8, 9, 11 };

      // Worker availability: 0 if the worker is unavailable for a shift
      double[,] availability =
          new double[,] { { 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 },
              { 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0 },
              { 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
              { 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 },
              { 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 },
              { 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1 },
              { 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };

      // Model
      GRBEnv env = new GRBEnv();
      GRBModel model = new GRBModel(env);
      model.Set(GRB.StringAttr.ModelName, "assignment");

      // Assignment variables: x[w][s] == 1 if worker w is assigned
      // to shift s. Since an assignment model always produces integer
      // solutions, we use continuous variables and solve as an LP.
      GRBVar[,] x = new GRBVar[nWorkers,nShifts];
      for (int w = 0; w < nWorkers; ++w) {
        for (int s = 0; s < nShifts; ++s) {
          x[w,s] =
              model.AddVar(0, availability[w,s], pay[w], GRB.CONTINUOUS,
                           Workers[w] + "." + Shifts[s]);
        }
      }

      // The objective is to minimize the total pay costs
      model.Set(GRB.IntAttr.ModelSense, 1);

      // Update model to integrate new variables
      model.Update();

      // Constraint: assign exactly shiftRequirements[s] workers
      // to each shift s
      LinkedList<GRBConstr> reqCts = new LinkedList<GRBConstr>();
      for (int s = 0; s < nShifts; ++s) {
        GRBLinExpr lhs = 0.0;
        for (int w = 0; w < nWorkers; ++w)
          lhs.AddTerm(1.0, x[w,s]);
        GRBConstr newCt =
            model.AddConstr(lhs == shiftRequirements[s], Shifts[s]);
        reqCts.AddFirst(newCt);
      }

      // Optimize
      model.Optimize();
      int status = model.Get(GRB.IntAttr.Status);
      if (status == GRB.Status.UNBOUNDED) {
        Console.WriteLine("The model cannot be solved "
            + "because it is unbounded");
        return;
      }
      if (status == GRB.Status.OPTIMAL) {
        Console.WriteLine("The optimal objective is " +
            model.Get(GRB.DoubleAttr.ObjVal));
        return;
      }
      if ((status != GRB.Status.INF_OR_UNBD) &&
          (status != GRB.Status.INFEASIBLE)) {
        Console.WriteLine("Optimization was stopped with status " + status);
        return;
      }

      // Add slack variables to make the model feasible
      Console.WriteLine("The model is infeasible; adding slack variables");

      // Set original objective coefficients to zero
      model.SetObjective(new GRBLinExpr());

      // Add a new slack variable to each shift constraint so that the shifts
      // can be satisfied
      LinkedList<GRBVar> slacks = new LinkedList<GRBVar>();
      foreach (GRBConstr c in reqCts) {
        GRBColumn col = new GRBColumn();
        col.AddTerm(1.0, c);
        GRBVar newvar =
            model.AddVar(0, GRB.INFINITY, 1.0, GRB.CONTINUOUS, col,
                         c.Get(GRB.StringAttr.ConstrName) + "Slack");
        slacks.AddFirst(newvar);
      }

      // Solve the model with slacks
      model.Optimize();
      status = model.Get(GRB.IntAttr.Status);
      if ((status == GRB.Status.INF_OR_UNBD) ||
          (status == GRB.Status.INFEASIBLE) ||
          (status == GRB.Status.UNBOUNDED)) {
        Console.WriteLine("The model with slacks cannot be solved "
            + "because it is infeasible or unbounded");
        return;
      }
      if (status != GRB.Status.OPTIMAL) {
        Console.WriteLine("Optimization was stopped with status " + status);
        return;
      }

      Console.WriteLine("\nSlack values:");
      foreach (GRBVar sv in slacks) {
        if (sv.Get(GRB.DoubleAttr.X) > 1e-6) {
          Console.WriteLine(sv.Get(GRB.StringAttr.VarName) + " = " +
              sv.Get(GRB.DoubleAttr.X));
        }
      }

      // Dispose of model and env
      model.Dispose();
      env.Dispose();

    } catch (GRBException e) {
      Console.WriteLine("Error code: " + e.ErrorCode + ". " +
          e.Message);
    }
  }
}
