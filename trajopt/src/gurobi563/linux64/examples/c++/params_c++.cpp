/* Copyright 2013, Gurobi Optimization, Inc. */

/* Use parameters that are associated with a model.

   A MIP is solved for 5 seconds with different sets of parameters.
   The one with the smallest MIP gap is selected, and the optimization
   is resumed until the optimal solution is found.
*/

#include "gurobi_c++.h"
#include <cmath>
using namespace std;

double gap(GRBModel *model) throw(GRBException);

int
main(int argc,
     char *argv[])
{
  if (argc < 2)
  {
    cout << "Usage: params_c++ filename" << endl;
    return 1;
  }

  GRBEnv* env = 0;
  GRBModel *bestModel = 0, *m = 0;
  try
  {
    // Read model and verify that it is a MIP
    env = new GRBEnv();
    GRBModel base = GRBModel(*env, argv[1]);
    if (base.get(GRB_IntAttr_IsMIP) == 0)
    {
      cout << "The model is not an integer program" << endl;
      return 1;
    }

    // Set a 5 second time limit
    base.getEnv().set(GRB_DoubleParam_TimeLimit, 5);

    // Now solve the model with different values of MIPFocus,
    // using a pointer to save the best model
    double bestGap = GRB_INFINITY;
    for (int i = 0; i <= 3; ++i)
    {
      m = new GRBModel(base);
      m->getEnv().set(GRB_IntParam_MIPFocus, i);
      m->optimize();
      if (bestModel == 0 || bestGap > gap(m))
      {
        delete bestModel;
        bestModel = m;
        m = 0;
        bestGap = gap(bestModel);
      }
      else
      {
        delete m;
        m = 0;
      }
    }

    // Finally, reset the time limit and continue to solve the
    // best model to optimality
    bestModel->getEnv().set(GRB_DoubleParam_TimeLimit, GRB_INFINITY);
    bestModel->optimize();
    cout << "Solved with MIPFocus: " <<
    bestModel->getEnv().get(GRB_IntParam_MIPFocus) << endl;

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

  delete bestModel;
  delete m;
  delete env;
  return 0;
}

// Simple function to determine the MIP gap
double gap(GRBModel *model) throw(GRBException)
{
  if ((model->get(GRB_IntAttr_SolCount) == 0) ||
      (fabs(model->get(GRB_DoubleAttr_ObjVal)) < 1e-6))
  {
    return GRB_INFINITY;
  }
  return fabs(model->get(GRB_DoubleAttr_ObjBound) -
              model->get(GRB_DoubleAttr_ObjVal)) /
         fabs(model->get(GRB_DoubleAttr_ObjVal));
}
