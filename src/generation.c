#include "generation.h"
#include "error.h"

// Create new generation
int generation(timetable_t** tts, int n, problem_t* p){
    timetable_t** pool = (timetable_t**)safe_malloc(sizeof(timetable_t*) * 2 * n);
    int pool_size = mutate(tts, n, p, pool);
    for (int i = 0; i < n; i++){
        pool[pool_size] = copy_timetable(tts[i]);
        local_improvement(pool[pool_size], p);
        pool_size++;
    }
    roulette(tts, pool, n, pool_size, p);
    timetable_t* tt = best_timetable(tts, n, p);
    return count_score(tt, p);
}

// Get best timetable
timetable_t* best_timetable(timetable_t** tts, int n, problem_t* p){
    int tmp;
    int min = count_score(tts[0], p);
    int all = 0;
    timetable_t* min_tt = tts[0];
    for (int i = 1; i < n; i++){
        tmp = count_score(tts[i], p);
        all += tmp;
        if (tmp < min){
            min = tmp;
            min_tt = tts[i];
        }
    }
    printf("%d ",all/n);
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
void local_improvement(timetable_t* tt, problem_t* p){
    int c = 0;
    // N1 - Swap timeslots of two random courses
    while(1){
        int c1 = rand() % tt->size;
        int c2 = rand() % tt->size;
        while (c1 == c2)
            c2 = rand() % tt->size;

        int old_room = tt->courses[c2].room;
        int old_timeslot = tt->courses[c2].timeslot;
        tt->courses[c2].timeslot = -1;
        tt->courses[c2].room = -1;
        int c1_room = stays_feasible(tt, c1, old_timeslot, p);
        tt->courses[c2].timeslot = old_timeslot;
        tt->courses[c2].room = old_room;


        old_room = tt->courses[c1].room;
        old_timeslot = tt->courses[c1].timeslot;
        tt->courses[c1].timeslot = -1;
        tt->courses[c1].room = -1;
        int c2_room = stays_feasible(tt, c2, old_timeslot, p);
        tt->courses[c1].timeslot = old_timeslot;
        tt->courses[c1].room = old_room;
        if (c1_room != -1 && c2_room != -1){
            move_course(tt, c1, tt->courses[c2].timeslot, c1_room);
            move_course(tt, c2, old_timeslot, c2_room);
            break;
        } else {
            c++;
            if (c > tt->size * (tt->size/2))
                break;
        }
    }

    // N2 - Move random course to random slot
    c = 0;
    bool r = move_course_randomly(tt, rand() % tt->size, p);
    while (!r){
        c++;
        if (c > tt->size * tt->size/2)
            break; // Do not try to move until end of time
        r = move_course_randomly(tt, rand() % tt->size, p);
    }

    // N3 - Swap all courses of two random timeslots
    signed t1 = rand() % TIMESLOTS;
    signed t2 = rand() % TIMESLOTS;
    while (t1 == t2)
        t2 = rand() % TIMESLOTS;
    for(int i = 0; i < tt->size; i++){
        if (tt->courses[i].timeslot == t1)
            tt->courses[i].timeslot = t2;
        else if (tt->courses[i].timeslot == t2)
            tt->courses[i].timeslot = t1;
    }

    //TODO N4 - Rotate events in randomly selected part of timetable
    signed tj = rand() % TIMESLOTS;
    signed ti = rand() % TIMESLOTS;
    while (ti == tj)
        ti = rand() % TIMESLOTS;
    if (ti > tj){
        int old = ti;
        ti =tj;
        tj = old;
    }
    for(int i = 0; i < tt->size; i++){
        if (tt->courses[i].timeslot == ti)
            tt->courses[i].timeslot = tj;
        else if (tt->courses[i].timeslot > ti && tt->courses[i].timeslot <= tj)
            tt->courses[i].timeslot = tt->courses[i].timeslot - 1;
    }

    //

    //TODO N5 - Move highest penalty course from 10% of random to random slot
    //TODO N6 - N5 with 20%
    //TODO N7 - N5 but to slot, that generates lowest penalty
    //TODO N8 - N7 with 20 percents

    //TODO N9 - kempe chain on random course and random timeslot
    //TODO N10 - N9 but use highest penalty course from 5% of random courses
    //TODO N11 - N10 with 20%
}

// Roulette wheel for new  population
void roulette(timetable_t** tts, timetable_t** pool, int basic_size, int pool_size, problem_t *p){
    // Find and sum all scores
    int sum = 0;
    int min = 1000000000;
    for (int i = 0; i < basic_size; i++){
        tmp2[i] = count_score(tts[i], p);
        sum += tmp2[i];
        if (tmp2[i] < min)
            min = tmp2[i];
    }
    for (int i = 0; i < pool_size; i++){
        tmp2[basic_size + i] = count_score(pool[i], p);
        sum += tmp2[basic_size + i];
        if (tmp2[basic_size + i] < min)
            min = tmp2[basic_size + i];
    }
    int to_del = pool_size;
    for (int i = 0; i < to_del; i++){
        int gen = rand() % sum;
        bool found = false;
        for (int j = 0; j < basic_size; j++) {
            gen -= tmp2[j];
            if (gen <= 0) {
                if (tmp2[j] > min) {
                    // FIXME
                    //delete_timetable(tts[j]);
                    tts[j] = pool[pool_size - 1];
                    sum -= tmp2[j];
                    tmp2[j] = tmp2[basic_size + pool_size - 1];
                    pool_size--;
                } else {
                    // FIXME
                    //delete_timetable(pool[pool_size - 1]);
                    sum -= tmp2[basic_size + pool_size - 1];
                    pool_size--;
                }
                found = true;
                break;
            }
        }
        if (found)
            continue;
        for (int j = 0; j < pool_size; j++) {
            gen -= tmp2[basic_size + j];
            if (gen <= 0) {
                if (tmp2[basic_size + j] > min) {
                    //FIXME
                    //delete_timetable(pool[j]);
                    sum -= tmp2[basic_size + j];
                    if (j != pool_size -1){
                        pool[j] = pool[pool_size - 1];
                        tmp2[basic_size + j] = tmp2[basic_size + pool_size - 1];
                    }
                    pool_size--;
                } else { // minimal is in pool, that is not fine, keep it
                    //FIXME
                    //delete_timetable(tts[0]);
                    tts[0] = pool[j];
                    sum -= tmp2[0];
                    tmp2[0] = tmp2[basic_size + j];
                    pool_size--;
                }
                found = true;
                break;
            }
        }
    }
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
                tmp1[count] = tt->courses[j].timeslot; // save timeslot
                count++;
            }
        }
        qsort(tmp1, count, sizeof(int), compare);

        // Student has only one course a day
        if (count_between(tmp1, 0, 8, count) == 1)
            score++;
        if (count_between(tmp1, 9, 17, count) == 1)
            score++;
        if (count_between(tmp1, 18, 26, count) == 1)
            score++;
        if (count_between(tmp1, 27, 35, count) == 1)
            score++;
        if (count_between(tmp1, 36, 44, count) == 1)
            score++;

        // More than two consecutive courses
        score += count_conseq(tmp1, 0, 8, count);
        score += count_conseq(tmp1, 9, 17, count);
        score += count_conseq(tmp1, 18, 26, count);
        score += count_conseq(tmp1, 27, 35, count);
        score += count_conseq(tmp1, 36, 44, count);

        // Student has course on the last timeslot of day
        if (has_value(tmp1, 8, count))
            score++;
        if (has_value(tmp1, 17, count))
            score++;
        if (has_value(tmp1, 26, count))
            score++;
        if (has_value(tmp1, 35, count))
            score++;
        if (has_value(tmp1, 44, count))
            score++;
    }
    return score;
}
