#include "generation.h"
#include "error.h"

// Create new generation
int generation(timetable_t** tts, int n, problem_t* p){
    timetable_t** pool = (timetable_t**)safe_malloc(sizeof(timetable_t*) * 2 * n);
    int pool_size = mutate(tts, n, p, pool);
    for (int i = 0; i < n; i++)
        local_improvement(tts[i]);
    roulette(tts, pool, n, pool_size);
    timetable_t* tt = best_timetable(tts, n, p);
    return count_score(tt, p);
}

// Get best timetable
timetable_t* best_timetable(timetable_t** tts, int n, problem_t* p){
    int tmp;
    int min = count_score(tts[0], p);
    timetable_t* min_tt = tts[0];
    for (int i = 1; i < n; i++){
        tmp = count_score(tts[i], p);
        if (tmp < min){
            min = tmp;
            min_tt = tts[i];
        }
    }
    return min_tt;
}

// Mutate 20% of courses in 20% of timetables
int mutate(timetable_t** tts, int n, problem_t* p, timetable_t** pool){
    int new = 0;
    for (int i = 0; i < n/5; i++) { // 20% of timetables will be mutated
        timetable_t* mutant = tts[rand() % n];
        pool[new] = copy_timetable(mutant);
        new++;
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
    return new;
}

// Find improved solutions
void local_improvement(timetable_t* tt){
    //TODO
}

// Roulette wheel for new  population
void roulette(timetable_t** tts, timetable_t** pool, int basic_size, int pool_size){
    //TODO
}

int count_conseq(int* tmp, int from, int to, int count){
    int last_value = -2;
    int current_streak = 0;
    int score = 0;
    for (int i = 0; i < count; i++){
        if (tmp[i] < from){
            last_value = tmp[i];
            continue;
        }
        if (tmp[i] > to){
            break;
        }
        if (tmp[i] - last_value == 1){
            current_streak++;
        } else {
            if (current_streak >= 2){
                score += current_streak - 1;
            }
            current_streak = 0;
        }
        last_value = tmp[i];
    }
    if (current_streak >= 2)
        score += current_streak - 1;
    return score;
}

// Count score of timetable
int count_score(timetable_t* tt, problem_t* p){
    int score = 0;
    for(int s = 0; s < students_count(p); s++){
        // Find all timeslots
        int count = 0;
        int* se = student_events(p, s);
        for (int j = 0; j < tt->size; j++){
            if (se[j] == 1){ // Student has course
                tmp[count] = tt->courses[j].timeslot; // save timeslot
                count++;
            }
        }
        qsort(tmp, count, sizeof(int), compare);

        // Student has only one course a day
        if (count_between(tmp, 0, 8, count) == 1)
            score++;
        if (count_between(tmp, 9, 17, count) == 1)
            score++;
        if (count_between(tmp, 18, 26, count) == 1)
            score++;
        if (count_between(tmp, 27, 35, count) == 1)
            score++;
        if (count_between(tmp, 36, 44, count) == 1)
            score++;

        // More than two consecutive courses
        score += count_conseq(tmp, 0, 8, count);
        score += count_conseq(tmp, 9, 17, count);
        score += count_conseq(tmp, 18, 26, count);
        score += count_conseq(tmp, 27, 35, count);
        score += count_conseq(tmp, 36, 44, count);

        // Student has course on the last timeslot of day
        if (has_value(tmp, 8, count))
            score++;
        if (has_value(tmp, 17, count))
            score++;
        if (has_value(tmp, 26, count))
            score++;
        if (has_value(tmp, 35, count))
            score++;
        if (has_value(tmp, 44, count))
            score++;
    }
    return score;
}
