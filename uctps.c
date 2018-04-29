/*
 * University timetabling problem solver
 *
 * Based on hybrid evolutionary approach
 *
 * Author: Matej Marusak
 */

#include <time.h>
#include <stdlib.h>
#include "src/dll.h"
#include "src/problem.h"
#include "src/solution_finder.h"

int main(int argc, char **argv) {
    if (argc != 2){
        printf("Incorrect usage\n");
        return 1;
    }
    int seed = time(NULL);
    srand(seed);

    FILE *fp;
    fp = fopen(argv[1], "r");
    problem_t* p = problem(fp);
    fclose(fp);

    //print_problem(p);

    init_solver();

    timetable_t* tt = timetable(events_count(p));
    bool found = find_feasible_timetable(p, &tt);
    while (!found){
        delete_timetable(tt);
        tt = timetable(events_count(p));
        found = find_feasible_timetable(p, &tt);
    }
    printf("FOUND\n");
    print_timetable(tt);

    delete_timetable(tt);
    delete_problem(p);
    return 0;
}
