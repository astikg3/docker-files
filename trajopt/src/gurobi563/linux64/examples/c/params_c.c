/* Copyright 2013, Gurobi Optimization, Inc. */

/* Use parameters that are associated with a model.

   A MIP is solved for 5 seconds with different sets of parameters.
   The one with the smallest MIP gap is selected, and the optimization
   is resumed until the optimal solution is found.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gurobi_c.h"

int gap(GRBmodel*, double*);

int
main(int   argc,
     char *argv[])
{
  GRBenv   *env   = NULL, *baseenv = NULL, *menv = NULL, *bestenv = NULL;
  GRBmodel *base  = NULL, *m = NULL, *bestModel = NULL;
  int       error = 0;
  int       i;
  int       ismip;
  int       mipfocus;
  double    bestGap = GRB_INFINITY, currGap;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: params_c filename\n");
    exit(1);
  }

  error = GRBloadenv(&env, "params.log");
  if (error) goto QUIT;

  /* Read model and verify that it is a MIP */
  error = GRBreadmodel(env, argv[1], &base);
  if (error) goto QUIT;
  error = GRBgetintattr(base, "IsMIP", &ismip);
  if (error) goto QUIT;
  if (ismip == 0)
  {
    printf("The model is not an integer program\n");
    goto QUIT;
  }

  /* Set a 5 second time limit */
  baseenv = GRBgetenv(base);
  if (!baseenv) goto QUIT;
  error = GRBsetdblparam(baseenv, "TimeLimit", 5);
  if (error) goto QUIT;

  /* Now solve the model with different values of MIPFocus */
  for (i = 0; i <= 3; ++i)
  {
    m = GRBcopymodel(base);
    if (!m) goto QUIT;
    menv = GRBgetenv(m);
    if (!menv) goto QUIT;
    error = GRBsetintparam(menv, "MIPFocus", i);
    if (error) goto QUIT;
    error = GRBoptimize(m);
    if (error) goto QUIT;
    error = gap(m, &currGap);
    if (error) goto QUIT;
    if (bestModel == NULL || bestGap > currGap)
    {
      GRBfreemodel(bestModel);
      bestModel = m;
      bestGap = currGap;
    }
    else
    {
      GRBfreemodel(m);
    }
  }

  /* Finally, reset the time limit and continue to solve the
     best model to optimality */
  bestenv = GRBgetenv(bestModel);
  if (!bestenv) goto QUIT;
  error = GRBsetdblparam(bestenv, "TimeLimit", GRB_INFINITY);
  if (error) goto QUIT;
  error = GRBoptimize(bestModel);
  if (error) goto QUIT;
  error = GRBgetintparam(bestenv, "MIPFocus", &mipfocus);
  if (error) goto QUIT;

  printf("Solved with MIPFocus: %i\n", mipfocus);

QUIT:

  /* Error reporting */

  if (error)
  {
    printf("ERROR: %s\n", GRBgeterrormsg(env));
    exit(1);
  }

  /* Free models */

  GRBfreemodel(bestModel);
  GRBfreemodel(base);

  /* Free environment */

  GRBfreeenv(env);

  return 0;
}


/* Simple function to determine the MIP gap */
int gap(GRBmodel *model, double *gap)
{
  int error;
  int solcount;
  double objval, objbound;

  error = GRBgetintattr(model, "SolCount", &solcount);
  if (error) return error;
  error = GRBgetdblattr(model, "ObjVal", &objval);
  if (error) return error;

  if ((solcount == 0) || (fabs(objval) < 1e-6))
  {
    *gap = GRB_INFINITY;
    return 0;
  }
  error = GRBgetdblattr(model, "ObjBound", &objbound);
  if (error) return error;

  *gap = fabs(objbound - objval) / fabs(objval);
  return 0;
}
