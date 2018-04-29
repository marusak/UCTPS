#include "timetable.h"
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

void delete_timetable(timetable_t *tt){
    free(tt->courses);
    free(tt);
}

void print_timetable(timetable_t *tt){
    for (int i = 0; i < tt->size; i++){
        printf("%d %d\n",tt->courses[i].timeslot, tt->courses[i].room);
    }
}
