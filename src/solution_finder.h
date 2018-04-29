/*
 * Find solutions for the problem
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_SOLUTION_FINDER_H
#define UCTPS_SOLUTION_FINDER_H

#include <stdbool.h>
#include "timetable.h"
#include "problem.h"

int numbers[TIMESLOTS];
int uncolored_students;
int student_count;

// Student/Color graph
typedef struct scg_t{
    dll_t* courses; //courses
    dll_t* colors; // free colors
    int colors_n;
    int courses_n;
}sgt_t;

sgt_t copy_student(sgt_t *old);

// Find random timetable that does not break any hard constraints
bool find_feasible_timetable(problem_t* p, timetable_t** tt);

// Scramble array of integers
void scramble();

// Initialize solver
void init_solver();

// Prepare student with all of his/her courses
void create_student(sgt_t** s, problem_t* p, int student_id);

// Prepare all students with all of their courses
sgt_t* create_students(problem_t *p);

// Count how many events aren't assigned yet
int uncolored_n(dll_t* courses);

// Check is student has course
bool has_course(sgt_t student, int course_id);

// Check if student does not have planed anything in given timeslot
bool has_free_color(sgt_t student, int color);

// Check if student has any free course
bool has_free_course(sgt_t *student);

// Check if course can be set it given timeslot
bool is_colorable(item *course, int color, sgt_t* students);

// Set course into given timeslot
void color_up(item *course, int color, sgt_t* students);

// Remove course from given timeslot
void uncolor_course(item *course, sgt_t* students);

#endif
