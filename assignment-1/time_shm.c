#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void* create_shared_mem(size_t size) {
    // Makes memory buffer readable and writable:
    int protection = PROT_READ | PROT_WRITE;
    // Only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    return mmap(NULL, size, protection, visibility, -1, 0);
}

int main(int argc, char* argv[]) {
    struct timeval* shared_mem = create_shared_mem(PAGE_SIZE);
    pid_t pid = fork();

    if (pid == 0) {
        struct timeval start_time;
        gettimeofday(&start_time, NULL);
        memcpy(shared_mem, &start_time, sizeof(start_time));
        execvp(argv[1], argv + 1);
    } else {
        wait(NULL);

        struct timeval end_time;
        struct timeval elapsed_time;

        gettimeofday(&end_time, NULL);
        timersub(&end_time, shared_mem, &elapsed_time);

        printf("\nElapsed time: %d.%06d seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
    }

    return 0;
}