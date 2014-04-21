/**
 * @author Nicolas Cami
 * @date 4 April 2014
 * @brief Simple program to compare algorithms that solve the n queens problem.
 *
 * L3 - 2013/2014 - Université de Perpignan Via Domitia
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define true 1
#define false 0
#define BEST_RATIO_HYBRID 3

/**
 * Define a solution
 */
typedef struct {
    int *value;
    int is_found;
} Solution;

/**
 * Global variables
 */
int       nb_solutions_tested;
Solution* solutions_set;
int       solutions_set_index;
int       nb_solutions;

/**
 * For debug purpose. Display t as an ASCII grid.
 * @param  {int*} t           The array.
 * @param  {int}  length      Array length.
 */
void print_grid(int* t, int length) {
    int i,j;

    for(j=0; j<length; j++)
        printf(" --");
    printf(" \n");
    for(i=0; i<length; i++) {
        printf("|");
        for(j=1; j<=length; j++) {
            if(t[i] == j) printf("<>|");
            else printf("  |");
        }
        printf("\n");
        for(j=0; j<length; j++)
            printf(" --");
        printf(" \n");
    }
    for(i=0; i<length; i++) {
        printf("%d ",t[i]);
    }
    printf("\n");
}

/*
 * Set a solution as "found". Used by randomized methods.
 * @param  {int*} t           The array.
 * @param  {int}  length      Array length.
 */
void add_solution(int *t, int n) {
    int i,j;

    for(i=0; i<nb_solutions; i++) {
        for(j=0; j<n; j++) {
            if(t[j] != solutions_set[i].value[j]) {
                break;
            }
        }
        // If this solution exists :
        if(j == n) {
            if(solutions_set[i].is_found == false) {
                solutions_set[i].is_found = true;
                solutions_set_index++;
            }
            break;
        }
    }
}

/*
 * Check if queens don't share a same diagonal.
 * @param  {int*} t           The array.
 * @param  {int}  length      Array length.
 * @return {int}  true|false
 */
int check_diagonal(int* t, int length) {
    int i,j;
    for(i=0; i<length; i++) {
        for(j=i+1; j<length; j++) {
            if(abs(t[i]-t[j]) == j-i) return false;
        }
    }
    return true;
}

/*
 * Swap two array items.
 * @param  {int*} t           The array.
 * @param  {int}  index1
 * @param  {int}  index2
 */
void swap_array_items(int* t, int index1, int index2) {
    int swap = 0;
    swap = t[index1];
    t[index1] = t[index2];
    t[index2] = swap;
}

/*
 * Find the first solution using permutations.
 * @param  {int*} t           The array.
 * @param  {int}  index       The index to permute with. At the first call, it should be 0.
 * @param  {int}  length      Array length.
 * @return {int}  true|false
 */
int permutation(int* t, int index, int length) {
    int i = 0;
    if(index==length) {
        nb_solutions_tested++;
        if(check_diagonal(t,length)) {
            return true;
        }
        return false;
    }
    else
    for(i=index; i<length; i++) {
        swap_array_items(t, index, i);
        if(permutation(t,index+1,length)==true) return true;
        swap_array_items(t, index, i);
    }
    return false;
}

/*
 * Find all solutions using permutations.
 * @param  {int*} t           The array.
 * @param  {int}  index       The index to permute with. At the first call, it should be 0.
 * @param  {int}  length      Array length.
 */
void permutation_all_solutions(int* t, int index, int taille) {
    int i = 0;
    if(index==taille) {
        nb_solutions_tested++;
        if(check_diagonal(t,taille)) {
        }
    }
    else
    for(i=index; i<taille; i++) {
        swap_array_items(t, index, i);
        permutation(t,index+1,taille);
        swap_array_items(t, index, i);
    }
}

/**
 * Check all queens from the first row until the current_row.
 * @param  {int*} t
 * @param  {int}  current_row
 * @return {int}  true|false
 */
int check_rows_before(int* t, int current_row) {
    int i;
    // check all rows before the current row
    for(i=0; i<current_row-1; i++) {
        // check current row with row i
        if(t[i] == t[current_row-1]) return false; // check column
        if(abs(t[i]-t[current_row-1]) == current_row-1-i) return false; // check diagonal
    }
    return true;
}

/*
 * Find the first solution using backtrack method.
 * Start with the current_row.
 * @param  {int*} t           The array.
 * @param  {int}  current_row The row being processed. At the first call, it should be 1.
 * @param  {int}  length      Array length.
 * @return {int}  true|false
 */
int backtrack(int *t, int current_row, int length) {
    int i;
    if(current_row == length+1) {
        return true;
    }
    else {
        for(i=1; i<=length; i++) {
            t[current_row-1] = i;
            nb_solutions_tested++;
            if(check_rows_before(t, current_row)) {
                if(backtrack(t, current_row+1, length)) return true;
            }
        }
    }
    return false;
}

/*
 * Find all solutions using backtrack method.
 * Start with the current_row.
 * @param  {int*} t           The array.
 * @param  {int}  current_row The row being processed. At the first call, it should be 1.
 * @param  {int}  length      Array length.
 * @return {int}  true|false
 */
void backtrack_all_solutions(int *t, int k, int length) {
    // If solution found :
    if(k == length+1) {
        int i;
        for(i=0; i<length; i++) {
            solutions_set[solutions_set_index].value[i] = t[i];
        }
        solutions_set_index++;
    }
    else {
        int i;
        for(i=1; i<=length; i++) {
            t[k-1] = i;
            nb_solutions_tested++;
            if(check_rows_before(t,k)) {
                backtrack_all_solutions(t,k+1,length);
            }
        }
    }
}

/*
    utilisé par l'hybrid qui cherche toutes les solutions
    ce backtrack ajoute une solution lorsqu'elle est trouvée
*/
/*
 * Find all solutions using backtrack method.
 * Used by hybrid_all_solutions to
 * Start with the current_row.
 * @param  {int*} t           The array.
 * @param  {int}  current_row The row being processed.
 * @param  {int}  length      Array length.
 * @return {int}  true|false
 */
void backtrack_hybrid(int *t, int current_row, int length) {
    if(current_row == length+1) {
        // Solution found, now we check if it's already found.
        add_solution(t,length);
    }
    else {
        int i;
        for(i=1; i<=length; i++) {
            t[current_row-1] = i;
            nb_solutions_tested++;
            if(check_rows_before(t, current_row)) {
                backtrack_hybrid(t, current_row+1, length);
            }
        }
    }
}

/**
 * Count the number of solutions using backtrack method.
 * @param  {int*} t            The array.
 * @param  {int}  current_row  The row being processed. At the first call, it should be 1.
 * @param  {int}  length       Array length.
 * @return {int}  nb           Number of existing solutions.
 */
int count_solutions(int *t, int current_row, int length) {
    int nb = 0;
    if(current_row == length+1) {
        nb += 1;
    }
    else {
        int i;
        for(i=1; i<=length; i++) {
            t[current_row-1] = i;
            if(check_rows_before(t, current_row)) {
                nb += count_solutions(t, current_row+1, length);
            }
        }
    }
    return nb;
}

/**
 * Try to find a single solution using las vegas random method.
 * @param  {int} *t          The array.
 * @param  {int} max         Maximum number of attempts.
 * @param  {int} length      Array length.
 * @return {int} true|false  If a solution is is_found.
 */
int las_vegas(int *t, int max, int length) {
    int i,j,nb,k=1;
    do {
        nb = 0;
        for(i=1; i<=length; i++) {
            nb_solutions_tested++;
            t[k-1] = i;
            if(check_rows_before(t,k)) {
                nb++;
                if((rand()%nb)+1 == 1)
                    j = i;
            }
        }
        if(nb > 0) {
            t[k-1] = j;
            k++;
        }
    } while(nb!=0 && k!=max+1);
    if(nb > 0)
        return true;
    return false;
}

/**
 * Find a single solution using las vegas random method.
 * @param  {int} *t          The array.
 * @param  {int} max         Number of queens to place.
 * @param  {int} length      Array length.
 * @return {int} true|false  If a solution is is_found.
 */
int las_vegas_brute(int *t, int max, int length) {
    int success = false;
    while(success == false) {
        success = las_vegas(t, max, length);
    }
    return true;
}

/**
 * Find a single solution using las_vegas/backtrack hybrid method.
 * @param  {int} *t          The array.
 * @param  {int} max         Number of queens to place using las vegas.
 * @param  {int} length      Array length.
 * @return {int} true|false  If a solution is is_found.
 */
int hybrid(int *t, int max, int length) {
    int success = false;
    while(success == false) {
        las_vegas_brute(t, max, length);
        success = backtrack(t, max+1, length);
    }
    return true;
}

/**
 * Find all solutions using las_vegas/backtrack hybrid method.
 * @param  {int} *t          The array.
 * @param  {int} max         Number of queens to place using las vegas.
 * @param  {int} length      Array length.
 * @return {int} true|false  If a solution is is_found.
 */
void hybrid_all_solutions(int *t, int max, int length) {
    las_vegas_brute(t, max, length);
    backtrack_hybrid(t, max+1, length);
}

/**
 * Perform tests and generate output files for GnuPlot.
 */
int main() {
    int                    i, j, k, n;
    int                    *t;
    clock_t                t1, t2;
    float                  averageDelay;
    unsigned long long int averageCombinations;
    int                    nbTest = 100;
    FILE                   *fSol1, *fSoln, *fSolh;

    srand(time(NULL));

    fSol1 = fopen("sol1.data","w");
    fSoln = fopen("soln.data","w");
    fSolh = fopen("solh.data","w");
    if(fSol1 == NULL || fSoln == NULL || fSolh == NULL) {
        printf("IMPOSSIBLE D'OUVRIR LES FICHIERS DATAS GNUPLOT\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fSol1, "x perm bt lv h2 h3 h4 h5\n");
    fprintf(fSoln, "x perm tperm bt tbt lv tlv h2 th2 h3 th3 h4 th4 h5 th5\n");
    fprintf(fSolh, "x une toutes\n");

    /* FIND A SINGLE SOLUTION TESTS */
    printf("\n\n======== TROUVER 1 SOLUTION ========\n");
    for(n=7; n<=12; n+=1) {
        printf("\n\n====== N = %d ======\n",n);
        fprintf(fSol1, "%d ",n);
        t = malloc(sizeof(int)*n);
        for(i=0; i<n; i++)
            t[i] = i+1;
        nb_solutions_tested = 0;
        printf("====  PERMUTATIONS  ====\n");
        t1 = clock();
        permutation(t,0,n);
        t2 = clock();
        fprintf(fSol1, "%d ", nb_solutions_tested-1);
        printf("%d solutions testees avant premiere solution\n%f ms\n", nb_solutions_tested-1,(((float)t2 - (float)t1) / 1000000.0F ) * 1000);

        nb_solutions_tested = 0;
        printf("====  BACKTRACK  ====\n");
        t1 = clock();
        backtrack(t,1,n);
        t2 = clock();
        fprintf(fSol1, "%d ", nb_solutions_tested-1);
        printf("%d solutions testees avant premiere solution\n%f ms\n", nb_solutions_tested-1,(((float)t2 - (float)t1) / 1000000.0F ) * 1000);

        averageDelay = 0;
        averageCombinations = 0;
        printf("====  LAS VEGAS  ====\n");
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            las_vegas_brute(t,n,n);
            averageCombinations += nb_solutions_tested;
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSol1, "%lld ", averageCombinations);
        printf("%lld solutions testees avant premiere solution\n%f ms\n", averageCombinations, averageDelay);

        averageDelay = 0;
        averageCombinations = 0;
        printf("====  hybrid  ====\n");
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            hybrid(t, BEST_RATIO_HYBRID, n);
            averageCombinations += nb_solutions_tested;
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSol1, "%lld ", averageCombinations);
        printf("%lld solutions testees avant premiere solution\n%f ms\n", averageCombinations, averageDelay);
        fprintf(fSol1, "\n");
        free(t);
    }

    /* FIND ALL SOLUTIONS TESTS */
    printf("\n\n======== TROUVER TOUTES LES SOLUTIONS ========\n");
    for(n=5; n<=9; n+=1) {
        printf("\n\n====== N = %d ======\n",n);
        fprintf(fSoln, "%d ", n);
        t = malloc(sizeof(int)*n);
        nb_solutions = count_solutions(t, 1, n);
        solutions_set = malloc(sizeof(Solution)*nb_solutions);
        for(i=0; i<nb_solutions; i++) {
            solutions_set[i].value = malloc(sizeof(int)*n);
            solutions_set[i].is_found = false;
        }
        solutions_set_index = 0;
        printf("il y a %d solutions differentes\n", nb_solutions);

        for(i=0; i<n; i++)
            t[i] = i+1;
        nb_solutions_tested = 0;
        printf("====  PERMUTATIONS  ====\n");
        t1 = clock();
        permutation_all_solutions(t,0,n);
        t2 = clock();
        fprintf(fSoln, "%d ", nb_solutions_tested);
        fprintf(fSoln, "%d ", (int)(((float)t2 - (float)t1) / 1000000.0F ) * 1000);
        printf("%d solutions testees\n%f ms\n", nb_solutions_tested, (((float)t2 - (float)t1) / 1000000.0F ) * 1000);

        nb_solutions_tested = 0;
        printf("====  BACKTRACK  ====\n");
        t1 = clock();
        backtrack_all_solutions(t,1,n);
        t2 = clock();
        fprintf(fSoln, "%d ", nb_solutions_tested);
        fprintf(fSoln, "%d ", (int)(((float)t2 - (float)t1) / 1000000.0F ) * 1000);
        printf("%d solutions testees\n%f ms\n", nb_solutions_tested, (((float)t2 - (float)t1) / 1000000.0F ) * 1000);

        nbTest = 5;
        averageDelay = 0;
        averageCombinations = 0;
        printf("====  LAS VEGAS  ====\n");
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            solutions_set_index = 0;
            for(j=0; j<nb_solutions; j++) {
                solutions_set[j].is_found = false;
            }
            while(solutions_set_index != nb_solutions) {
                las_vegas_brute(t, n, n);
                add_solution(t, n);
            }
            averageCombinations += nb_solutions_tested;
            printf("test %d : nb combi = %d\n", i, nb_solutions_tested);
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSoln, "%lld ", averageCombinations);
        fprintf(fSoln, "%d ", (int)averageDelay);
        printf("%lld solutions testees\n%f ms\n", averageCombinations, averageDelay);

        printf("====  hybrid  ====\n");
        averageDelay = 0;
        averageCombinations = 0;
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            solutions_set_index = 0;
            for(j=0; j<nb_solutions; j++) {
                solutions_set[j].is_found = false;
            }
            while(solutions_set_index != nb_solutions) {
                hybrid_all_solutions(t, BEST_RATIO_HYBRID, n);
            }
            averageCombinations += nb_solutions_tested;
            printf("test %d : nb combi = %d\n", i, nb_solutions_tested);
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSoln, "%lld ", averageCombinations);
        fprintf(fSoln, "%d ", (int)averageDelay);
        printf("%lld solutions testees\n%f ms\n", averageCombinations, averageDelay);

        free(t);
        for(i=0; i<nb_solutions; i++) {
            free(solutions_set[i].value);
        }
        free(solutions_set);
        fprintf(fSoln, "\n");
    }

    /* HYBRID RATIO TESTS */
    printf("\n\n======== ANALYSE TAUX hybrid ========\n");
    n = 8;
    t = malloc(sizeof(int)*n);
    nb_solutions = count_solutions(t, 1, n);
    solutions_set = malloc(sizeof(Solution)*nb_solutions);
    for(i=0; i<nb_solutions; i++) {
        solutions_set[i].value = malloc(sizeof(int)*n);
        solutions_set[i].is_found = false;
    }
    solutions_set_index = 0;
    backtrack_all_solutions(t, 1, n);
    for(j=1; j<n; j++) {
        nbTest = 1000;
        averageDelay = 0;
        averageCombinations = 0;
        printf("====  hybrid %d  ====\n",j);
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            hybrid(t, j, n);
            averageCombinations += nb_solutions_tested;
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSolh, "%d %lld ", j, averageCombinations);
        printf("%lld solutions testees avant premiere solution\n%f ms\n", averageCombinations, averageDelay);

        nbTest = 10;
        averageDelay = 0;
        averageCombinations = 0;
        t1 = clock();
        for(i=0; i<nbTest; i++) {
            nb_solutions_tested = 0;
            solutions_set_index = 0;
            for(k=0; k<nb_solutions; k++) {
                solutions_set[k].is_found = false;
            }
            while(solutions_set_index != nb_solutions) {
                hybrid_all_solutions(t, j, n);
            }
            averageCombinations += nb_solutions_tested;
        }
        t2 = clock();
        averageDelay = ((((int)t2 - (int)t1) / 1000000 ) * 1000)/(float)nbTest;
        averageCombinations /= nbTest;
        fprintf(fSolh, "%lld\n", averageCombinations);
        printf("%lld solutions testees\n%f ms\n", averageCombinations, averageDelay);
    }
    free(t);
    for(i=0; i<nb_solutions; i++) {
        free(solutions_set[i].value);
    }
    free(solutions_set);

    fclose(fSol1);
    fclose(fSolh);
    fclose(fSoln);

    printf("\n\nFinish!\nYou can use GnuPlot : load config to display graphics.\n");

    return EXIT_SUCCESS;
}
