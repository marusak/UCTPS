#include "generation.h"
#include "error.h"

// Create new generation
int generation(timetable_t** tts, int n){
    mutate(tts);
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
void mutate(timetable_t** tts){
    //TODO
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
