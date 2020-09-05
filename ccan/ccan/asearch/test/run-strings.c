#include <ccan/asearch/asearch.h>
#include <ccan/array_size/array_size.h>
#include <ccan/tap/tap.h>
#include <stdlib.h>

#include <ccan/asearch/asearch.c>

static int cmp(const int *key, const char *const *elem, void *ctx)
{
	return *key - atoi(*elem);
}

int main(void)
{
	const char *args[] = { "1", "4", "7", "9" };
	int key = 7;
	const char **p;

	plan_tests(1);
	p = asearch(&key, args, ARRAY_SIZE(args), cmp, NULL);
	ok1(p == &args[2]);

	return exit_status();
}
