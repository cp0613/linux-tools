#include <stdio.h>

void foo()
{
	printf("hello, uprobe!\n");
}
int main()
{
	foo();
	return 0;
}
