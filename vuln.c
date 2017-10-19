#include <stdio.h>

int main(int argc, char **argv) {
    char buf[64];
	printf("What is the password? ");
	scanf("%s", buf);
	printf("%s???\nWRONG! No shell for you!\n"); // haha silly user
	return 0;
}
