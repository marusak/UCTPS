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
#include "src/generation.h"
#include "src/solution_finder.h"

int main(int argc, char **argv) {
    int seed = time(NULL);
    srand(seed);
    if(argc != 2){
        printf("Incorrect usage\n");
        return 1;
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    problem_t* p = problem(fp);
    fclose(fp);

    init_solver();

    int N = 100; // size of population
    timetable_t** tts = (timetable_t**)safe_malloc(sizeof(timetable_t*) * N);
    for (int i = 0; i < N; i++){
        timetable_t* tt = timetable(events_count(p));
        bool found = find_feasible_timetable(p, &tt);
        if (!found){
            printf("Solution not found\n");
            break;
        }
        tts[i] = tt;
    }

    int penalty = 100;
    int remaining_generations = 30;
    while(penalty != 0 && remaining_generations > 0){
        penalty = generation(tts, N, p);
        printf("%d\n",penalty);
        remaining_generations--;
    }

    timetable_t* tt = best_timetable(tts, N, p);
    print_timetable(tt);

    for(int i = 0; i < N; i++)
        delete_timetable(tts[i]);
    free(tts);
    delete_problem(p);
    return 0;
}
