typedef struct course_t {
    int timeslot; // color in graph coloring
    int room;
}course_t;

typedef struct timetable_t {
    int size;
    course_t* courses;
} timetable_t;

// Create new timetable
timetable_t* timetable(int courses_n);

// Set course's timeslot
void set_timeslot(timetable_t* tt, int course, int value);

// Set course's room
void set_room(timetable_t* tt, int course, int value);

//Output timetable (mainly for debugging)
void print_timetable(timetable_t* tt);

// Delete timetable
void delete_timetable(timetable_t* tt);

