
# include <unistd.h>
#include <stdlib.h>

int main()
{
	while(1)
	{
		system("leaks minishell");
		sleep(1);
	}
}