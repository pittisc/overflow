#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    char buf[256];
	if (argc != 2) {
		printf("USAGE: %s <password>\n", argv[0]);
		return 1;
	}
	strcpy(buf, argv[1]);
	printf("%s???\nWRONG! No shell for you!\n"); // haha silly user
	return 0;
}
