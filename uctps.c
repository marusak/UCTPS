/*
 * University timetabling problem solver
 *
 * Based on hybrid evolutionary approach
 *
 * Author: Matej Marusak
 */

#include "src/dll.h"
#include "src/problem.h"

int main() {
    dll_t* students= dll();
    item* x = insert_last(students, 10, 11);
    x = insert_after(students, x, 5, 15);
    x = insert_before(students, x, 8, 14);
    x = insert_first(students, 7, 14);
    x = insert_before(students, x, 1, 15);
    print_dll(students);
    teardown(students);

    FILE *fp;
    fp = fopen("test.txt", "r");
    problem_t* p = problem(fp);
    fclose(fp);

    print_problem(p);
    delete_problem(p);
    return 0;
}
