/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example reads an LP or a MIP from a file, sets a callback
   to monitor the optimization progress and to output some progress
   information to the screen and to a log file. If it is a MIP and 10%
   gap is reached, then it aborts */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gurobi_c.h"


/* Define structure to pass my data to the callback function */

struct callback_data {
  double  lastnode;
  double  lastiter;
  FILE   *logfile;
  double *solution;
};

int __stdcall
mycallback(GRBmodel *model,
           void     *cbdata,
           int       where,
           void     *usrdata)
{
  struct callback_data *mydata = (struct callback_data *) usrdata;
  char *msg;

  if (where == GRB_CB_POLLING) {
    /* Ignore polling callback */
  } else if (where == GRB_CB_MESSAGE) {
    GRBcbget(cbdata, where, GRB_CB_MSG_STRING, &msg);
    fprintf(mydata->logfile, "%s", msg);
  } else if (where == GRB_CB_SIMPLEX) {
    double pinf, dinf, obj, itrcnt;
    int    ispert;
    char   ch;
    GRBcbget(cbdata, where, GRB_CB_SPX_ITRCNT, &itrcnt);
    if (itrcnt - mydata->lastiter >= 100) {
      mydata->lastiter = itrcnt;
      GRBcbget(cbdata, where, GRB_CB_SPX_ISPERT, &ispert);
      GRBcbget(cbdata, where, GRB_CB_SPX_OBJVAL, &obj);
      GRBcbget(cbdata, where, GRB_CB_SPX_PRIMINF, &pinf);
      GRBcbget(cbdata, where, GRB_CB_SPX_DUALINF, &dinf);
      if      (ispert == 0) ch = ' ';
      else if (ispert == 1) ch = 'S';
      else                  ch = 'P';
      printf("%7.0f %14.7e%c %13.6e %13.6e\n", itrcnt, obj, ch, pinf, dinf);
    }
  } else if (where == GRB_CB_PRESOLVE) {
    int cdels, rdels;
    GRBcbget(cbdata, where, GRB_CB_PRE_COLDEL, &cdels);
    GRBcbget(cbdata, where, GRB_CB_PRE_ROWDEL, &rdels);
    if (cdels || rdels) {
      printf("%7d columns and %7d rows are removed\n", cdels, rdels);
    }
  } else if (where == GRB_CB_MIP) {
    double objbst, objbnd, nodecnt, actnodes, itrcnt;
    int    cutcnt, solcnt;
    GRBcbget(cbdata, where, GRB_CB_MIP_NODCNT, &nodecnt);
    if (nodecnt - mydata->lastnode >= 100) {
      mydata->lastnode = nodecnt;
      GRBcbget(cbdata, where, GRB_CB_MIP_OBJBST, &objbst);
      GRBcbget(cbdata, where, GRB_CB_MIP_OBJBND, &objbnd);
      /* if gap is small, then quit */
      if (fabs(objbst - objbnd) < 0.1 * (1.0 + fabs(objbst))) {
        printf("Stop early - 10%% gap achieved\n");
        GRBterminate(model);
      }
      GRBcbget(cbdata, where, GRB_CB_MIP_NODLFT, &actnodes);
      GRBcbget(cbdata, where, GRB_CB_MIP_ITRCNT, &itrcnt);
      GRBcbget(cbdata, where, GRB_CB_MIP_SOLCNT, &solcnt);
      GRBcbget(cbdata, where, GRB_CB_MIP_CUTCNT, &cutcnt);
      printf("%7.0f %7.0f %8.0f %13.6e %13.6e %7d %7d\n",
             nodecnt, actnodes, itrcnt, objbst, objbnd, solcnt, cutcnt);
    }
  } else if (where == GRB_CB_MIPSOL) {
    double obj, nodecnt;
    int solcnt;
    GRBcbget(cbdata, where, GRB_CB_MIPSOL_OBJ, &obj);
    GRBcbget(cbdata, where, GRB_CB_MIPSOL_NODCNT, &nodecnt);
    GRBcbget(cbdata, where, GRB_CB_MIPSOL_SOLCNT, &solcnt);
    GRBcbget(cbdata, where, GRB_CB_MIPSOL_SOL, mydata->solution);

    printf("**** New solution at node %.0f, obj %g, sol %d, x[0] = %.2f ****\n",
           nodecnt, obj, solcnt, mydata->solution[0]);
  } else if (where == GRB_CB_BARRIER) {
    double primobj, dualobj, priminf, dualinf, compl;
    int iter;
    GRBcbget(cbdata, where, GRB_CB_BARRIER_ITRCNT,  &iter);
    GRBcbget(cbdata, where, GRB_CB_BARRIER_PRIMOBJ, &primobj);
    GRBcbget(cbdata, where, GRB_CB_BARRIER_DUALOBJ, &dualobj);
    GRBcbget(cbdata, where, GRB_CB_BARRIER_PRIMINF, &priminf);
    GRBcbget(cbdata, where, GRB_CB_BARRIER_DUALINF, &dualinf);
    GRBcbget(cbdata, where, GRB_CB_BARRIER_COMPL,   &compl);

    printf("Barrier iter: %d %.4e %.4e %.4e %.4e %.4e\n",
           iter, primobj, dualobj, priminf, dualinf, compl);
  }
  return 0;
}

int
main(int   argc,
     char *argv[])
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       error = 0;
  int       vars, optimstatus;
  double    objval;
  struct callback_data mydata;

  mydata.lastnode = -GRB_INFINITY;
  mydata.lastiter = -GRB_INFINITY;
  mydata.logfile  = NULL;
  mydata.solution = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: callback_c filename\n");
    goto QUIT;
  }

  mydata.logfile = fopen("cb.log", "w");
  if (!mydata.logfile) {
    fprintf(stderr, "Cannot open cb.log for callback message\n");
    goto QUIT;
  }

  /* Create environment */

  error = GRBloadenv(&env, NULL);
  if (error) goto QUIT;

  /* Turn off display */

  error = GRBsetintparam(env, GRB_INT_PAR_OUTPUTFLAG, 0);
  if (error) goto QUIT;

  /* Read model from file */

  error = GRBreadmodel(env, argv[1], &model);
  if (error) goto QUIT;

  /* Allocate space for solution */

  error = GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &vars);
  if (error) goto QUIT;

  mydata.solution = malloc(vars*sizeof(double));
  if (mydata.solution == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(1);
  }

  /* Set callback function */

  error = GRBsetcallbackfunc(model, mycallback, (void *) &mydata);
  if (error) goto QUIT;

  /* Solve model */

  error = GRBoptimize(model);
  if (error) goto QUIT;

  /* Capture solution information */

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) goto QUIT;

  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) goto QUIT;

  printf("\nOptimization complete\n");
  if (optimstatus == GRB_OPTIMAL)
    printf("Optimal objective: %.4e\n", objval);
  else if (optimstatus == GRB_INF_OR_UNBD)
    printf("Model is infeasible or unbounded\n");
  else
    printf("Optimization was stopped early\n");
  printf("\n");

QUIT:

  /* Error reporting */

  if (error) {
    printf("ERROR: %s\n", GRBgeterrormsg(env));
    exit(1);
  }

  /* Close file */

  if (mydata.logfile)
    fclose(mydata.logfile);

  /* Free solution */

  if (mydata.solution)
    free(mydata.solution);

  /* Free model */

  GRBfreemodel(model);

  /* Free environment */

  GRBfreeenv(env);

  return 0;
}
