/*
 * Handling UT problem
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_PROBLEM_H
#define UCTPS_PROBLEM_H

//According to the dataset, always count only 45 timeslots
#define TIMESLOTS 45

#include <stdlib.h>
#include <stdio.h>
#include "dll.h"

//HACK - only first line of file is longer than one character
//In any given dataset 40 character on first line is much more than needed
#define BUF_SIZE 40

typedef struct problem {
    int n_events;
    int n_rooms;
    int n_features;
    int n_students;
    int* room_sizes;
    int* student_event;
    int* room_feature;
    int* event_feature;
} problem_t;

// Create new problem
problem_t* problem(FILE* fh);

// Get count of events
int events_count(problem_t *problem);

// Get count of rooms
int rooms_count(problem_t *problem);

// Get count of features
int features_count(problem_t *problem);

// Get count of students
int students_count(problem_t *problem);

// Get nth room size
int room_size(problem_t* problem, int n);

// Get nth student's events
int* student_events(problem_t* problem, int n);

// Get nth room's features
int* room_features(problem_t* problem, int n);

// Get nth event's features
int* event_features(problem_t* problem, int n);

// Get dll of student's events
dll_t* find_events(problem_t* p, int student_id, int* count);

// Get dll of all events
dll_t* get_events(problem_t* p);

// Delete problem
void delete_problem(problem_t* problem);

// Print the problem (mainly for debugging)
void print_problem(problem_t* problem);

#endif
