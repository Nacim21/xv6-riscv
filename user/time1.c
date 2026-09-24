#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include "kernel/stat.h"

int main(int argc, char *argv[])
{
    printf("First argument: %s\n", argv[1]);

    int pid = fork();
    if( pid < 0){
        printf("failed \n");
        return 1;
    }
    else if (pid == 0){
        exec(argv[1],&argv[1]);

        printf("execution of child failed \n");
        return 1;
    }
    else{
        int start = uptime();
	int status;
	struct rusage ru;
	wait2(&status, &ru);
        int end = uptime();

	int elapsed = end-start;

	printf("Elapsed time: %d , cpu time: %d, %d %%CPU \n", elapsed, ru.cputime, ru.cputime *100 /elapsed);
        exit(0);
    }

}
