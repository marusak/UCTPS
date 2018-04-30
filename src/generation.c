#include "generation.h"
#include "error.h"

// Create new generation
int generation(timetable_t** tts, int n, problem_t* p){
    mutate(tts, n, p);
    for (int i = 0; i < n; i++)
        local_improvement(tts[i]);
    roulette(tts, n);
    timetable_t* tt = best_timetable(tts, n);
    return count_score(tt);
}

// Get best timetable
timetable_t* best_timetable(timetable_t** tts, int n){
    int tmp;
    int min = count_score(tts[0]);
    timetable_t* min_tt = tts[0];
    for (int i = 1; i < n; i++){
        tmp = count_score(tts[i]);
        if (tmp < min){
            min = tmp;
            min_tt = tts[i];
        }
    }
    return min_tt;
}

// Mutate 20% of courses in 20% of timetables
void mutate(timetable_t** tts, int n, problem_t* p){
    for (int i = 0; i < n/5; i++) { // 20% of timetables will be mutated
        timetable_t* mutant = tts[rand() % n];
        int c = 0;
        for (int j = 0; j < mutant->size/5; j++){ // 20% courses
            bool r = move_course_randomly(mutant, rand() % mutant->size, p);
            while (!r){
                c++;
                if (c > mutant->size * mutant->size/5)
                    break; // Do not try to mutate until end of time
                r = move_course_randomly(mutant, rand() % mutant->size, p);
            }
        }
    }
}

// Find improved solutions
void local_improvement(timetable_t* tt){
    //TODO
}

// Roulette wheel for new  population
void roulette(timetable_t** tts, int n){
    //TODO
}

// Count score of timetable
int count_score(timetable_t* tt){
    //TODO
    return 5;
}
