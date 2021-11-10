#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void run_imp_actual(int64_t* values);

void print_values(int64_t* arr, size_t N) {
    for(size_t i = 0; i < N; i++) {
        printf("%lli ", (long long)arr[i]);
    }
    printf("\n");
}

int main(int argc, char const* argv[]) {
    // Holds values
    int64_t values[16384] = {0};
    for (int i = 1; i < argc; i++) {
        values[i - 1] = atoll(argv[i]);
    }

    printf("Initial values:\n\t");
    print_values(values, argc - 1);
    run_imp_actual(values);
    printf("Final values:\n\t");
    print_values(values, argc - 1);
    return 0;
}
