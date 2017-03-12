#include "SAT.h"
#include <stdio.h>
#include <unistd.h>

/*
 * libsat-demo2.c -- another example of use of libsat.a 
 * We also suppress output to stdout, and then re-enable it only after
 * we are ready to output our result.
 *
 * NOTE: this program does very poor error-checking!
 *  
 * To run this program, place it in the zchaff folder. Then, compile
 * the executable by issuing: g++ libsat-demo2.c -L. -lsat -o libsat-demo2
 * 
 */


int main() {
    int p[2], bak;

    pipe(p);
    bak = dup(STDOUT_FILENO);
    dup2(p[1], STDOUT_FILENO);

    SAT_Manager mgr = SAT_InitManager();

    SAT_SetNumVariables(mgr, 3);
    int c[3];

    c[0] = (1 << 1); c[1] = (2 << 1); c[2] = (3 << 1);
    SAT_AddClause(mgr, c, 3);

    c[0] = (1 << 1) + 1;
    SAT_AddClause(mgr, c, 1);

    c[0] = (2 << 1) + 1;
    SAT_AddClause(mgr, c, 1);

    /*c[0] = (3 << 1) + 1;
    SAT_AddClause(mgr, c, 1);
*/
    int result = SAT_Solve(mgr);
    dup2(bak, STDOUT_FILENO);
    if(result == SATISFIABLE) {
	printf("sat: "); fflush(stdout);
	int n = SAT_NumVariables(mgr);
	int i;

	for(i = 1; i <= n; i++) {
	    int a = SAT_GetVarAsgnment(mgr, i);
	    if(a == 1) {
		printf("%d ", i); fflush(stdout);
	    }
	    else if(a == 0) {
		printf("%d ", -1*i); fflush(stdout);
	    }
	    else {
		printf("Error!"); fflush(stdout);
	    }
	}
	printf("\n"); 
    }
    else {
	printf("unsat.\n"); fflush(stdout);
    }
 
    return 0;
}
