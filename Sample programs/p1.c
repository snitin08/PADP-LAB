#include <omp.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
#pragma omp parallel
    {
        printf("Hello World\n");
        printf("How are you\n\n");
    }
}
