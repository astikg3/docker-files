/* Copyright 2013, Gurobi Optimization, Inc. */

/* Simple MIP sensitivity analysis example.
   For each integer variable, fix it to its lower and upper bound
   and check the impact on the objective. */

#include "gurobi_c++.h"
using namespace std;

int
main(int argc,
     char *argv[])
{
  if (argc < 2)
  {
    cout << "Usage: sensitivity_c++ filename" << endl;
    return 1;
  }

  GRBEnv* env = 0;
  GRBVar* avars = 0;
  GRBVar* bv = 0;
  try
  {
    // Read model
    env = new GRBEnv();
    GRBModel a = GRBModel(*env, argv[1]);
    a.optimize();
    a.getEnv().set(GRB_IntParam_OutputFlag, 0);

    // Extract variables from model
    avars = a.getVars();

    for (int i = 0; i < a.get(GRB_IntAttr_NumVars); ++i)
    {
      GRBVar v = avars[i];
      if (v.get(GRB_CharAttr_VType) == GRB_BINARY)
      {

        // Create clone and fix variable
        GRBModel b = GRBModel(a);
        bv = b.getVars();
        if (v.get(GRB_DoubleAttr_X) - v.get(GRB_DoubleAttr_LB) < 0.5)
        {
          bv[i].set(GRB_DoubleAttr_LB, bv[i].get(GRB_DoubleAttr_UB));
        }
        else
        {
          bv[i].set(GRB_DoubleAttr_UB, bv[i].get(GRB_DoubleAttr_LB));
        }
        delete[] bv;
        bv = 0;

        b.optimize();

        if (b.get(GRB_IntAttr_Status) == GRB_OPTIMAL)
        {
          double objchg =
            b.get(GRB_DoubleAttr_ObjVal) - a.get(GRB_DoubleAttr_ObjVal);
          if (objchg < 0)
          {
            objchg = 0;
          }
          cout << "Objective sensitivity for variable " <<
          v.get(GRB_StringAttr_VarName) << " is " << objchg << endl;
        }
        else
        {
          cout << "Objective sensitivity for variable " <<
          v.get(GRB_StringAttr_VarName) << " is infinite" << endl;
        }
      }
    }

  }
  catch (GRBException e)
  {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  }
  catch (...)
  {
    cout << "Error during optimization" << endl;
  }

  delete[] avars;
  delete[] bv;
  delete env;
  return 0;
}
