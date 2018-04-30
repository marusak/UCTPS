/*
 * Timetable structure and related methods
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_TIMETABLE_H
#define UCTPS_TIMETABLE_H

#include <stdbool.h>
#include "problem.h"

typedef struct course_t {
    int timeslot; // color in graph coloring
    int room;
}course_t;

typedef struct timetable_t {
    int size;
    course_t* courses;
} timetable_t;

int tmp[500];

// Create new timetable
timetable_t* timetable(int courses_n);

// Set course's timeslot
void set_timeslot(timetable_t* tt, int course, int value);

// Set course's room
void set_room(timetable_t* tt, int course, int value);

//Output timetable (mainly for debugging)
void print_timetable(timetable_t* tt);

// Move selected course to random position to keep feasible solution
bool move_course_randomly(timetable_t* tt, int course_id, problem_t* p);

// Check if course in new_timeslot would be feasible
int stays_feasible(timetable_t* tt, int course_id, int new_timeslot, problem_t* p);

// Move course to new timeslot
void move_course(timetable_t* tt, int course_id, int new_timeslot, int new_room);

// Delete timetable
void delete_timetable(timetable_t* tt);

#endif
