/*
 * Create new generation
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_GENERATION_H
#define UCTPS_GENERATION_H

#include <stdbool.h>
#include "timetable.h"
#include "problem.h"

int tmp[500];

// Create new generation
int generation(timetable_t** tts, int n, problem_t* p);

// Get best timetable
timetable_t* best_timetable(timetable_t** tts, int n, problem_t* p);

// Mutate 20% of courses in 20% of timetables
void mutate(timetable_t** tts, int n, problem_t* p);

// Find improved solutions
void local_improvement(timetable_t* tt);

// Roulette wheel for new  population
void roulette(timetable_t** tts, int n);

// Count score of timetable
int count_score(timetable_t* tt, problem_t* p);

#endif
