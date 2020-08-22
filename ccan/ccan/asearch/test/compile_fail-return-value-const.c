#include <ccan/asearch/asearch.h>
#include <ccan/array_size/array_size.h>
#include <string.h>

#include <ccan/asearch/asearch.c>

static int cmp(const char *key, const char *const *elem, void *ctx)
{
	return strcmp(key, *elem);
}

int main(void)
{
	const char key[] = "key";
	const char *elems[] = { "a", "big", "list", "of", "things" };

#ifdef FAIL
	char **p;
#if !HAVE_TYPEOF
#error "Unfortunately we don't fail if no typeof."
#endif
#else
	const char **p;
#endif
	p = asearch(key, elems, ARRAY_SIZE(elems), cmp, NULL);
	return p ? 0 : 1;
}
