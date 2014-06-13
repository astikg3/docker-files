/* Copyright 2013, Gurobi Optimization, Inc. */

/* Simple MIP sensitivity analysis example.
   For each integer variable, fix it to its lower and upper bound
   and check the impact on the objective. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gurobi_c.h"

int
main(int   argc,
     char *argv[])
{
  GRBenv   *env = NULL, *aenv;
  GRBmodel *a = NULL, *b = NULL;
  int       error = 0;
  int       i, numvars, status;
  char      vtype, *vname;
  double    x, bnd, aobj, bobj, objchg;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: sensitivity_c filename\n");
    exit(1);
  }

  error = GRBloadenv(&env, "sensitivity.log");
  if (error) goto QUIT;

  /* Read model */
  error = GRBreadmodel(env, argv[1], &a);
  if (error) goto QUIT;
  error = GRBoptimize(a);
  if (error) goto QUIT;
  error = GRBgetdblattr(a, "ObjVal", &aobj);
  if (error) goto QUIT;
  aenv = GRBgetenv(a);
  if (!aenv) goto QUIT;
  error = GRBsetintparam(aenv, "OutputFlag", 0);
  if (error) goto QUIT;

  /* Iterate over all variables */
  error = GRBgetintattr(a, "NumVars", &numvars);
  if (error) goto QUIT;
  for (i = 0; i < numvars; ++i)
  {
    error = GRBgetcharattrelement(a, "VType", i, &vtype);
    if (error) goto QUIT;

    if (vtype == GRB_BINARY)
    {

      /* Create clone and fix variable */
      b = GRBcopymodel(a);
      if (!b) goto QUIT;
      error = GRBgetstrattrelement(a, "VarName", i, &vname);
      if (error) goto QUIT;
      error = GRBgetdblattrelement(a, "X", i, &x);
      if (error) goto QUIT;
      error = GRBgetdblattrelement(a, "LB", i, &bnd);
      if (error) goto QUIT;
      if (x - bnd < 0.5)
      {
        error = GRBgetdblattrelement(b, "UB", i, &bnd);
        if (error) goto QUIT;
        error = GRBsetdblattrelement(b, "LB", i, bnd);
        if (error) goto QUIT;
      }
      else
      {
        error = GRBgetdblattrelement(b, "LB", i, &bnd);
        if (error) goto QUIT;
        error = GRBsetdblattrelement(b, "UB", i, bnd);
        if (error) goto QUIT;
      }

      error = GRBoptimize(b);
      if (error) goto QUIT;

      error = GRBgetintattr(b, "Status", &status);
      if (error) goto QUIT;
      if (status == GRB_OPTIMAL)
      {
        error = GRBgetdblattr(b, "ObjVal", &bobj);
        if (error) goto QUIT;
        objchg = bobj - aobj;
        if (objchg < 0)
        {
          objchg = 0;
        }
        printf("Objective sensitivity for variable %s is %f\n", vname, objchg);
      }
      else
      {
        printf("Objective sensitivity for variable %s is infinite\n", vname);
      }

      GRBfreemodel(b);
      b = NULL;
    }
  }


QUIT:

  /* Error reporting */

  if (error)
  {
    printf("ERROR: %s\n", GRBgeterrormsg(env));
    exit(1);
  }

  /* Free models */

  GRBfreemodel(a);
  GRBfreemodel(b);

  /* Free environment */

  GRBfreeenv(env);

  return 0;
}
