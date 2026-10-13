#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
	printf("Up %d clock ticks",uptime());
	exit(0);
}
