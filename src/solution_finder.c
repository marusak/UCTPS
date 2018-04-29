#include "solution_finder.h"
#include "dll.h"
#include "error.h"

void scramble(){
    for (int i = 0; i < TIMESLOTS ; i++){
        unsigned j = rand() % TIMESLOTS;
        int t = numbers[j];
        numbers[j] = numbers[i];
        numbers[i] = t;
    }
}

void init_solver(){
    for(int i = 0; i< TIMESLOTS; i++){
        numbers[i] = i;
    }
}

void create_student(sgt_t** s, problem_t* p, int student_id){
    sgt_t* new_s = *s;
    new_s->courses = find_events(p, student_id, &(new_s->courses_n));

    dll_t* colors = dll();
    scramble();
    for (int i=0; i < TIMESLOTS; i++)
        insert_last(colors, numbers[i], -1);
    new_s->colors = colors;
    new_s->colors_n = TIMESLOTS;
}

sgt_t* create_students(problem_t *p){
    sgt_t* new_s = (sgt_t*)safe_malloc(sizeof(sgt_t) * students_count(p));
    sgt_t* return_p = new_s;
    for (int i = 0; i < students_count(p); i++){
        create_student(&new_s, p, i);
        new_s++;
    }
    return return_p;
}

int uncolored_n(dll_t* courses){
    int count = 0;
    item* current = courses->first;
    while (current != NULL){
        if (current->count == -1)
            count++;
        current = current->next;
    }
    return count;
}

bool has_course(sgt_t student, int course_id){
    item* current = student.courses->first;
    while (current != NULL){
        if (current->value == course_id)
            return true;
        current = current->next;
    }
    return false;
}

bool has_free_color(sgt_t student, int color){
    item* current = student.colors->first;
    while (current != NULL){
        if (current->value == color){
            if (current->count == -1)
                return true;
            break;
        }
        current = current->next;
    }
    return false;
}

bool has_free_course(sgt_t *student){
    item* current = student->courses->first;
    while (current != NULL){
        if (current->count == -1)
            return true;
        current = current->next;
    }
    return false;
}

bool is_colorable(item *course, int color, sgt_t* students){
    for (int i = 0; i < student_count; i++){
        if (has_course(students[i], course->value)){
            if (!has_free_color(students[i], color))
                return false;
        }
    }
    return true;
}

void cleanup_students(sgt_t* students){
    for (int i = 0; i < student_count; i++){
        teardown(students[i].colors);
        teardown(students[i].courses);
    }
    free(students);
}

void color_up(item *course, int color, sgt_t* students){
    for (int i = 0; i < student_count; i++){
        if (has_course(students[i], course->value)){
            if (color == -1){
                int old_value = get_count(students[i].courses, course->value);
                set_count_to_value(students[i].colors, old_value, -1);
            }
            else
                set_count_to_value(students[i].colors, color, course->value);
            set_count_to_value(students[i].courses, course->value, color);
        }
    }
    course->count = color;
}

void uncolor_course(item *course, sgt_t* students){
    color_up(course, -1, students);
}

int count_uncolored_students(sgt_t* students){
    int count = 0;
    for (int i = 0; i < student_count; i++){
        if (has_free_course(&students[i]))
            count++;
    }
    return count;
}

bool exists_unresolved_course(dll_t* timeslot){
    item* cur = timeslot->first;
    while (cur != NULL){
        if (cur->count == -1)
            return true;
        cur = cur->next;
    }
    return false;
}

item* unresolved_with_lowest_values(dll_t* timeslot, dll_t** all_rooms, int* count){
    int min = 100000000;
    item* min_item = NULL;
    item* cur = timeslot->first;
    while (cur != NULL){
        if (cur->count == -1){
            int count = dll_len_with_count(all_rooms[cur->value], -1);
            if (count < min){
                min = count;
                min_item = cur;
            }
        }
        cur = cur->next;
    }
    *count = min;
    return min_item;
}

dll_t* find_appropraite_rooms(problem_t *p, int course_id){
    dll_t* new_list = dll();
    int students_count = students_count_in_course(p, course_id);
    for (int i = 0; i < rooms_count(p); i++)
        if (room_size(p, i) >= students_count)
            if (event_fits_room(p, course_id, i))
                insert_last(new_list, i, -1);
    return new_list;
}

item* random_fiting_room(dll_t* rooms, int count){
    int req = rand() % count;
    int cnt = 0;
    item* cur = rooms->first;
    while (cur != NULL){
        if (cur->count == -1){
            if (cnt == req)
                return cur;
            cnt++;
            }
        cur = cur->next;
    }
    error("Internal error", INTERNAL_ERROR);
    return NULL;
}

void cleanup_rooms(dll_t** rooms, int count){
    for (int i = 0; i < count; i++){
        item *cur = rooms[i]->first;
        while (cur != NULL){
            cur->count = -1;
            cur = cur->next;
        }
    }
}

void set_up_rooms(dll_t** rooms, int count, int req, int value){
    for (int i = 0; i < count; i++){
        item *cur = rooms[i]->first;
        while (cur != NULL){
            if (cur->value == req)
                cur->count = value;
            cur = cur->next;
        }
    }
}

bool find_feasible_timetable(problem_t *p, timetable_t** tt){
    item* s_course = NULL;

    /* Step 1: Graph Coloring
     *
     * Select random student, select random color, that the student does not yet
     * used (color is timeslot) and select uncolored course that he/she takes.
     * Try coloring this course (all students of that course had to have that
     * color free. If possible to color, do so, otherwise select random colored
     * course and remove it's color. Repeat until all courses (and students) are
     * colored.
     */

    // Prepare all students with free courses and RANDOM free colors
    sgt_t* students = create_students(p);
    uncolored_students = students_count(p);
    student_count = uncolored_students;

    // List of timeslots, with accepted rooms
    dll_t** timeslots = (dll_t**)safe_malloc(sizeof(dll_t*) * TIMESLOTS);
    for (int i = 0; i < TIMESLOTS; i++)
        timeslots[i] = dll();

    // Create list of courses with appropriate rooms
    dll_t** rooms = (dll_t**)safe_malloc(sizeof(dll_t*) * events_count(p));
    for (int i = 0; i < events_count(p); i++)
        rooms[i] = find_appropraite_rooms(p, i);

    int count = 0; // we cannot try constructing feasible solution until end of time
    while (uncolored_students > 0 && count < 5000){ // TODO debug this magic number
        // Select random student
        sgt_t* s = students + rand() % students_count(p);

        uncolored_students = count_uncolored_students(students);

        // Select random student's free course
        if (has_free_course(s))
            while (true) {
                s_course = get_nth(s->courses, rand() % s->courses_n);
                if (s_course->count == -1)
                    break;
            }
        else
            continue;

        bool colored = false;
        for (int i = 0; i < s->colors_n; i++){
            item* color_i = get_nth(s->colors, i);
            if (color_i->count != -1)
                continue;
            int color = color_i->value;
            if (is_colorable(s_course, color, students)){
                color_up(s_course, color, students);
                colored = true;
                break;
            }
        }
        if (!colored){
            count++;
            item* victim = get_nth(s->courses, rand() % s->courses_n);
            uncolor_course(victim, students);
        }
    }

    if (count >= 5000){
        cleanup_students(students);
        return false;
    }

    // Colored graph, convert into timetable
    for (int i = 0; i < student_count; i++){
        item* cur = students[i].courses->first;
        while (cur != NULL){
            if ((*tt)->courses[cur->value].timeslot != -1  && (*tt)->courses[cur->value].timeslot != cur->count)
                error("Internal error", INTERNAL_ERROR);
            (*tt)->courses[cur->value].timeslot = cur->count;
            insert_last_if_not_in(timeslots[cur->count], cur->value, -1);
            cur = cur->next;
        }
    }

    /* Step 2: Check if timeslot is room coverable
     *
     * For each timeslot (one color from step 1):
     *     For each course count, in how many rooms can be
     *     Order courses from lowest in count of possible rooms
     *     Select one random of possible rooms and assigned it. Again recalculate
     *     possible rooms. If some unassigned course reaches 0, it is not feasible
     *     solution and we need to start all over.
     */

    // For each timeslot
    for (int i = 0; i < TIMESLOTS; i++){
        count = 0;
        while (exists_unresolved_course(timeslots[i]) && count < 1000){ //debug this magic number
            // Select unresolved course with lowest untaken rooms assigned
            int rooms_count;
            item* course = unresolved_with_lowest_values(timeslots[i], rooms, &rooms_count);
            if (! course || rooms_count == 0){
                count++;
                clean_dll(timeslots[i]);
                cleanup_rooms(rooms, events_count(p));
                continue;
            }
            item* room = random_fiting_room(rooms[course->value], rooms_count);
            set_up_rooms(rooms, events_count(p), room->value, course->value);
            course->count = room->value;
        }
        if (count >= 1000)
            return false;
        cleanup_rooms(rooms, events_count(p));
    }

    // Clean up students
    cleanup_students(students);

    // Clean up timeslots
    for (int i = 0; i < TIMESLOTS; i++)
        teardown(timeslots[i]);
    free(timeslots);

    return true;
}
