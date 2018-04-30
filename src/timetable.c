#include "timetable.h"
#include "problem.h"
#include "utils.h"

timetable_t* timetable(int courses_n){
    timetable_t* new_tt = (timetable_t*)safe_malloc(sizeof(timetable_t));
    new_tt->courses = (course_t*)safe_malloc(sizeof(course_t) * courses_n);
    for (int i = 0; i < courses_n; i++){
        new_tt->courses[i].timeslot = -1;
        new_tt->courses[i].room = -1;
    }
    new_tt->size = courses_n;
    return new_tt;
}

void set_timeslot(timetable_t *tt, int course, int value){
    tt->courses[course].timeslot = value;
}

void set_room(timetable_t *tt, int course, int value){
    tt->courses[course].room = value;
}

bool move_course_randomly(timetable_t* tt, int course_id, problem_t* p){
    bool moved = false;
    int i = tt->courses[course_id].timeslot;
    for (int j = 0; j < TIMESLOTS; j++) {
        int new_room = stays_feasible(tt, course_id, (j+i)%TIMESLOTS, p);
        if (new_room > -1){
            move_course(tt, course_id, (j+i)%TIMESLOTS, new_room);
            moved = true;
            break;
        }
    }
    return moved;
}

int stays_feasible(timetable_t* tt, int course_id, int new_timeslot, problem_t* p){
    // Count needed capacity
    int needed_capacity = 0;
    for(int s = 0; s < students_count(p); s++){
        int* se = student_events(p, s);
        if (se[course_id] == 1)
            needed_capacity++;
    }

    // Find all events in the new timeslot
    int conflicting_events = 0;
    for(int i = 0; i < tt->size; i++){
        if (tt->courses[i].timeslot == new_timeslot){
            tmp[conflicting_events] = i;
            conflicting_events++;
        }
    }

    // Check if in given slot some student does not have any conflict
    for(int s = 0; s < students_count(p); s++){
        int* se = student_events(p, s);
        int c = 0;
        for (int j = 0; j < conflicting_events; j++){
            if (se[tmp[j]] == 1)
                c++;
        }
        if (se[course_id] == 1)
            c++;
        if (c > 1)
            return -1;
    }

    // Check if exists free room, that has enough capacity and satisfy features
    for(int i = 0; i < p->n_rooms; i++){
        for(int j = 0; j < conflicting_events; j++){
            if (tt->courses[tmp[j]].room == i)
                continue;
            else { // Not used room
                if (needed_capacity > room_size(p,i))
                    continue;
                else {
                    if (event_fits_room(p, course_id, i))
                        return i;
                    else
                        continue;
                }

            }
        }
    }
    return -1;
}

void move_course(timetable_t* tt, int course_id, int new_timeslot, int new_room){
    tt->courses[course_id].timeslot = new_timeslot;
    tt->courses[course_id].room = new_room;
}

void delete_timetable(timetable_t *tt){
    free(tt->courses);
    free(tt);
}

void print_timetable(timetable_t *tt){
    for (int i = 0; i < tt->size; i++){
        printf("%d %d\n",tt->courses[i].timeslot, tt->courses[i].room);
    }
}
