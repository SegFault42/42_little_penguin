#include <stdio.h>

typedef struct		s_test
{
	char		*a;
	char		*b;
	char		*c;
}			t_test;

int	main()
{
	t_test	lol = {
		.a = "rabougue",
		.b = "Ramzi",
		.c = "Bouguerra"
	};
	printf("%s\n", lol.b);
}
