#include <stdio.h>
#include <unistd.h>

int main() {
	printf("UserID: %d\n", getuid());
	printf("GroupID: %d\n", getgid());
	return 0;
}
