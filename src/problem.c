/*
 * Handling UT problem
 *
 * Author: Matej Marusak
 */

#include "problem.h"
#include "utils.h"

problem_t* problem(FILE *fl){
    problem_t* np = malloc(sizeof(problem_t));
    char* buffer = malloc(sizeof(char) * (BUF_SIZE + 1));
    char* rest;
    int* itr;

    get_line(buffer, BUF_SIZE, fl);
    rest = buffer;

    // Read first line containing all parameters
    np->n_events = get_number(&rest);
    if (!np->n_events)
        exit(3);
    rest++;

    np->n_rooms = get_number(&rest);
    if (!np->n_rooms)
        exit(3);
    rest++;

    np->n_features = get_number(&rest);
    if (!np->n_features)
        exit(3);
    rest++;

    np->n_students = get_number(&rest);
    if (!np->n_students)
        exit(3);

    // Read all room_sizes
    np->room_sizes = safe_malloc(sizeof(int) * (np->n_rooms));
    itr = np->room_sizes;
    for (int i = 0; i < np->n_rooms; i++){
        get_line(buffer, BUF_SIZE, fl);
        rest = buffer;
        *(itr) = get_number(&rest);
        if (!*(itr))
            exit(3);
        itr++;
    }

    // Read values for student/event
    np->student_event = safe_malloc(sizeof(int) * (np->n_students * np->n_events));
    itr = np->student_event;
    for (int i = 0; i < np->n_students * np->n_events; i++){
        get_line(buffer, BUF_SIZE, fl);
        *(itr) = get_binary(buffer[0]);
        itr++;
    }

    // Read values for room/feature
    np->room_feature = safe_malloc(sizeof(int) * (np->n_rooms * np->n_features));
    itr = np->room_feature;
    for (int i = 0; i < np->n_rooms * np->n_features; i++){
        get_line(buffer, BUF_SIZE, fl);
        *(itr) = get_binary(buffer[0]);
        itr++;
    }

    // Read values for event/feature
    np->event_feature = safe_malloc(sizeof(int) * (np->n_events * np->n_features));
    itr = np->event_feature;
    for (int i = 0; i < np->n_events * np->n_features; i++){
        get_line(buffer, BUF_SIZE, fl);
        *(itr) = get_binary(buffer[0]);
        itr++;
    }

    free(buffer);
    return np;
}

// Get count of events
int events_count(problem_t *problem){
    return problem->n_events;
}

// Get count of rooms
int rooms_count(problem_t *problem){
    return problem->n_rooms;
}

// Get count of features
int features_count(problem_t *problem){
    return problem->n_features;
}

// Get count of students
int students_count(problem_t *problem){
    return problem->n_students;
}

// Get nth room size
int room_size(problem_t* problem, int n){
    return problem->room_sizes[n];
}

// Get nth student's events
int* student_events(problem_t* problem, int n){
    return problem->student_event + n * problem->n_events;
}

// Get nth room's features
int* room_features(problem_t* problem, int n){
    return problem->room_feature + n * problem->n_features;
}

// Get nth event's features
int* event_features(problem_t* problem, int n){
    return problem->event_feature + n * problem->n_features;
}

void delete_problem(problem_t* problem){
    free(problem->room_sizes);
    free(problem->student_event);
    free(problem->room_feature);
    free(problem->event_feature);
    free(problem);
}

void print_problem(problem_t* problem){
    char* tmp;
    printf("Events: %d\nRooms: %d\nFeatures: %d\nStudents: %d\n\n",
            problem->n_events, problem->n_rooms, problem->n_events, problem->n_students);
    printf("Rooms sizes: ");
    for (int i = 0; i < problem->n_rooms; i++)
        printf("%d ", room_size(problem, i));
    printf("\nStudents/events:\n");
    for (int i = 0; i < problem->n_students; i++){
        tmp = ints_to_string(student_events(problem, i), problem->n_events);
        printf("%s\n", tmp);
        free(tmp);
    }
    printf("\nRoom/Features:\n");
    for (int i = 0; i < problem->n_rooms; i++){
        tmp = ints_to_string(room_features(problem, i), problem->n_features);
        printf("%s\n", tmp);
        free(tmp);
    }
    printf("\nEvent/Features:\n");
    for (int i = 0; i < problem->n_events; i++){
        tmp = ints_to_string(event_features(problem, i), problem->n_features);
        printf("%s\n", tmp);
        free(tmp);
    }
}
