/* If SINGLE is defined when triangle.o is compiled, it should also be       */
/*   defined here.  If not, it should not be defined here.                   */

/* #define SINGLE */

#ifdef SINGLE
#define TRIREAL float
#else /* not SINGLE */
#define TRIREAL double
#endif /* not SINGLE */

#include "triangle.h"

#include <R.h>
#include <Rdefines.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
     
void
R_init_triangle(DllInfo *info)
{
  /* Register routines, allocate resources. */
}
     
void
R_unload_triangle(DllInfo *info)
{
  /* Release resources. */
}

/*****************************************************************************/
/*                                                                           */
/*  report()   Print the input or output.                                    */
/*                                                                           */
/*****************************************************************************/

void report(io, markers, reporttriangles, reportneighbors, reportsegments,
            reportedges, reportnorms)
struct triangulateio *io;
int markers;
int reporttriangles;
int reportneighbors;
int reportsegments;
int reportedges;
int reportnorms;
{
  int i, j;

  for (i = 0; i < io->numberofpoints; i++) {
    printf("Point %4d:", i);
    for (j = 0; j < 2; j++) {
      printf("  %.6g", io->pointlist[i * 2 + j]);
    }
    if (io->numberofpointattributes > 0) {
      printf("   attributes");
    }
    for (j = 0; j < io->numberofpointattributes; j++) {
      printf("  %.6g",
             io->pointattributelist[i * io->numberofpointattributes + j]);
    }
    if (markers) {
      printf("   marker %d\n", io->pointmarkerlist[i]);
    } else {
      printf("\n");
    }
  }
  printf("\n");

  if (reporttriangles || reportneighbors) {
    for (i = 0; i < io->numberoftriangles; i++) {
      if (reporttriangles) {
        printf("Triangle %4d points:", i);
        for (j = 0; j < io->numberofcorners; j++) {
          printf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
        }
        if (io->numberoftriangleattributes > 0) {
          printf("   attributes");
        }
        for (j = 0; j < io->numberoftriangleattributes; j++) {
          printf("  %.6g", io->triangleattributelist[i *
                                         io->numberoftriangleattributes + j]);
        }
        printf("\n");
      }
      if (reportneighbors) {
        printf("Triangle %4d neighbors:", i);
        for (j = 0; j < 3; j++) {
          printf("  %4d", io->neighborlist[i * 3 + j]);
        }
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportsegments) {
    for (i = 0; i < io->numberofsegments; i++) {
      printf("Segment %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->segmentlist[i * 2 + j]);
      }
      if (markers) {
        printf("   marker %d\n", io->segmentmarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportedges) {
    for (i = 0; i < io->numberofedges; i++) {
      printf("Edge %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->edgelist[i * 2 + j]);
      }
      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
        for (j = 0; j < 2; j++) {
          printf("  %.6g", io->normlist[i * 2 + j]);
        }
      }
      if (markers) {
        printf("   marker %d\n", io->edgemarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }
}

/*****************************************************************************/
/*                                                                           */
/*  main()   Create and refine a mesh.                                       */
/*                                                                           */
/*****************************************************************************/

int tricall()
{
  struct triangulateio in, mid, out, vorout;

  /* Define input points. */

  in.numberofpoints = 4;
  in.numberofpointattributes = 1;
  in.pointlist = (TRIREAL *) malloc(in.numberofpoints * 2 * sizeof(TRIREAL));
  in.pointlist[0] = 0.0;
  in.pointlist[1] = 0.0;
  in.pointlist[2] = 1.0;
  in.pointlist[3] = 0.0;
  in.pointlist[4] = 1.0;
  in.pointlist[5] = 10.0;
  in.pointlist[6] = 0.0;
  in.pointlist[7] = 10.0;
  in.pointattributelist = (TRIREAL *) malloc(in.numberofpoints *
                                          in.numberofpointattributes *
                                          sizeof(TRIREAL));
  in.pointattributelist[0] = 0.0;
  in.pointattributelist[1] = 1.0;
  in.pointattributelist[2] = 11.0;
  in.pointattributelist[3] = 10.0;
  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
  in.pointmarkerlist[0] = 0;
  in.pointmarkerlist[1] = 2;
  in.pointmarkerlist[2] = 0;
  in.pointmarkerlist[3] = 0;

  in.numberofsegments = 0;
  in.numberofholes = 0;
  in.numberofregions = 1;
  in.regionlist = (TRIREAL *) malloc(in.numberofregions * 4 * sizeof(TRIREAL));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 5.0;
  in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
  in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */

  printf("Input point set:\n\n");
  report(&in, 1, 0, 0, 0, 0, 0);

  /* Make necessary initializations so that Triangle can return a */
  /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

  mid.pointlist = (TRIREAL *) NULL;            /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of point attributes is zero: */
  mid.pointattributelist = (TRIREAL *) NULL;
  mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
  mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  mid.triangleattributelist = (TRIREAL *) NULL;
  mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
  /* Needed only if segments are output (-p or -c) and -P not used: */
  mid.segmentlist = (int *) NULL;
  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
  mid.segmentmarkerlist = (int *) NULL;
  mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
  mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

  vorout.pointlist = (TRIREAL *) NULL;        /* Needed only if -v switch used. */
  /* Needed only if -v switch used and number of attributes is not zero: */
  vorout.pointattributelist = (TRIREAL *) NULL;
  vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
  vorout.normlist = (TRIREAL *) NULL;         /* Needed only if -v switch used. */

  /* Triangulate the points.  Switches are chosen to read and write a  */
  /*   PSLG (p), preserve the convex hull (c), number everything from  */
  /*   zero (z), assign a regional attribute to each element (A), and  */
  /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
  /*   neighbor list (n).                                              */

  triangulate("pczAevn", &in, &mid, &vorout);

  printf("Initial triangulation:\n\n");
  report(&mid, 1, 1, 1, 1, 1, 0);
  printf("Initial Voronoi diagram:\n\n");
  report(&vorout, 0, 0, 0, 0, 1, 1);

  /* Attach area constraints to the triangles in preparation for */
  /*   refining the triangulation.                               */

  /* Needed only if -r and -a switches used: */
  mid.trianglearealist = (TRIREAL *) malloc(mid.numberoftriangles * sizeof(TRIREAL));
  mid.trianglearealist[0] = 3.0;
  mid.trianglearealist[1] = 1.0;

  /* Make necessary initializations so that Triangle can return a */
  /*   triangulation in `out'.                                    */

  out.pointlist = (TRIREAL *) NULL;            /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of attributes is zero: */
  out.pointattributelist = (TRIREAL *) NULL;
  out.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  out.triangleattributelist = (TRIREAL *) NULL;

  /* Refine the triangulation according to the attached */
  /*   triangle area constraints.                       */

  triangulate("prazBP", &mid, &out, (struct triangulateio *) NULL);

  printf("Refined triangulation:\n\n");
  report(&out, 0, 1, 0, 0, 0, 0);

  /* Free all allocated arrays, including those allocated by Triangle. */

  free(in.pointlist);
  free(in.pointattributelist);
  free(in.pointmarkerlist);
  free(in.regionlist);
  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.trianglearealist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);
  free(vorout.pointlist);
  free(vorout.pointattributelist);
  free(vorout.edgelist);
  free(vorout.normlist);
  free(out.pointlist);
  free(out.pointattributelist);
  free(out.trianglelist);
  free(out.triangleattributelist);

  return 0;
}

SEXP R_triangulate (SEXP P, SEXP PB, SEXP S, SEXP SB, SEXP a)
{
  SEXP Q;
  SEXP T;
  SEXP ans, dimnames;
  double *xP, *xQ;
  int *xT;
  int nP, nQ, nT;
  
  /* Convert input point matrix into array */
  PROTECT(P = AS_NUMERIC(P));
  xP = REAL(P);
  /* Convert input boundary markers into array */
  // PROTECT(B = AS_NUMERIC(B));

  /* Create the triangulateio structures */
  struct triangulateio in, mid, out, vorout;

  in.numberofpoints = LENGTH(P)/2;
  in.numberofpointattributes = 0; 
  // in.pointlist = (TRIREAL *) malloc(in.numberofpoints * 2 * sizeof(TRIREAL));
  in.pointlist = REAL(P);
  in.pointmarkerlist = INTEGER(PB);
  
  
  in.numberofsegments = LENGTH(S)/2;
  in.segmentlist = INTEGER(S);
  in.segmentmarkerlist = INTEGER(SB);
  in.numberofholes = 0;
  in.numberofregions = 0;

  printf("Input point set:\n\n");
  report(&in, 1, 0, 0, 1, 0, 0);

  /* Make necessary initializations so that Triangle can return a */
  /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

  mid.pointlist = (TRIREAL *) NULL;            /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of point attributes is zero: */
  mid.pointattributelist = (TRIREAL *) NULL;
  mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
  mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  mid.triangleattributelist = (TRIREAL *) NULL;
  mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
  /* Needed only if segments are output (-p or -c) and -P not used: */
  mid.segmentlist = (int *) NULL;
  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
  mid.segmentmarkerlist = (int *) NULL;
  mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
  mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

  vorout.pointlist = (TRIREAL *) NULL;        /* Needed only if -v switch used. */
  /* Needed only if -v switch used and number of attributes is not zero: */
  vorout.pointattributelist = (TRIREAL *) NULL;
  vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
  vorout.normlist = (TRIREAL *) NULL;         /* Needed only if -v switch used. */

  /* Triangulate the points.  Switches are chosen to read and write a  */
  /*   PSLG (p), preserve the convex hull (c), assign a regional       */
  /*   attribute to each element (A), and                              */
  /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
  /*   neighbor list (n).                                              */

  char flags[100];
  strcpy(flags, "pevn");
  char opts[100];
  if (isReal(a)) {
    sprintf(opts, "a%f", *REAL(a));
    strcat(flags, opts);
    printf(flags);
  }

  triangulate(flags, &in, &mid, &vorout);

  printf("Initial triangulation:\n\n");
  report(&mid, 1, 1, 1, 1, 1, 0);
  printf("Initial Voronoi diagram:\n\n");
  report(&vorout, 0, 0, 0, 0, 1, 1);

  /* Attach area constraints to the triangles in preparation for */
  /*   refining the triangulation.                               */

  /* /\* Needed only if -r and -a switches used: *\/ */
  /* mid.trianglearealist = (TRIREAL *) malloc(mid.numberoftriangles * sizeof(TRIREAL)); */
  /* mid.trianglearealist[0] = 3.0; */
  /* mid.trianglearealist[1] = 1.0; */

  /* /\* Make necessary initializations so that Triangle can return a *\/ */
  /* /\*   triangulation in `out'.                                    *\/ */

  /* out.pointlist = (TRIREAL *) NULL;            /\* Not needed if -N switch used. *\/ */
  /* /\* Not needed if -N switch used or number of attributes is zero: *\/ */
  /* out.pointattributelist = (TRIREAL *) NULL; */
  /* out.trianglelist = (int *) NULL;          /\* Not needed if -E switch used. *\/ */
  /* /\* Not needed if -E switch used or number of triangle attributes is zero: *\/ */
  /* out.triangleattributelist = (TRIREAL *) NULL; */

  /* /\* Refine the triangulation according to the attached *\/ */
  /* /\*   triangle area constraints.                       *\/ */

  /* triangulate("praBP", &mid, &out, (struct triangulateio *) NULL); */

  /* printf("Refined triangulation:\n\n"); */
  /* report(&out, 0, 1, 0, 0, 0, 0); */

  /* Make space for answers */
  printf("Number of output points %d", mid.numberofpoints);
  PROTECT(Q = allocMatrix(REALSXP, mid.numberofpoints, 2));
  PROTECT(T = allocMatrix(INTSXP, mid.numberoftriangles, 3));

  xQ = REAL(Q);
  for (int i = 0; i < mid.numberofpoints; i++) {
    for (int j = 0; j < 2; j++) {
      xQ[j * mid.numberofpoints + i] = mid.pointlist[i * 2 + j];
    }
  }
  
  xT = INTEGER(T);
  for (int i = 0; i < mid.numberoftriangles; i++) {
    for (int j = 0; j < mid.numberofcorners; j++) {
      xT[j * mid.numberoftriangles + i] = mid.trianglelist[i * mid.numberofcorners + j];
    }
  }
  
  PROTECT(ans = allocVector(VECSXP, 2));
  SET_VECTOR_ELT(ans, 0, Q);
  SET_VECTOR_ELT(ans, 1, T);

  UNPROTECT(4);

  /* Free all allocated arrays, including those allocated by Triangle. */
  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.trianglearealist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);
  free(vorout.pointlist);
  free(vorout.pointattributelist);
  free(vorout.edgelist);
  free(vorout.normlist);
  /*  free(out.pointlist);
  free(out.pointattributelist);
  free(out.trianglelist);
  free(out.triangleattributelist); */

  return(ans);
} 