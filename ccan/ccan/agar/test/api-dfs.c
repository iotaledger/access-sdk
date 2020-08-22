#include "config.h"

#include <stddef.h>
#include <assert.h>

#include <ccan/tal/tal.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <ccan/ptrint/ptrint.h>

#include <ccan/agar/agar.h>

#include "simple-graphr.h"

#define test_dfs_partial(sr, first, ...)				\
	do {								\
		int cmp[] = { __VA_ARGS__ };				\
		bool stillok = true;					\
		const void *node;					\
		int i = 0;						\
		agar_dfs(node, (sr), int2ptr(first)) {			\
			int index = ptr2int(node);			\
			diag("Visited %d\n", index);			\
			if (i >= ARRAY_SIZE(cmp) || (index != cmp[i]))	\
				stillok = false;			\
			i++;						\
		}							\
		ok1(stillok);						\
	} while (0)

#define test_dfs(gr, first, ...)					\
	do {								\
		struct agar_state *sr;					\
		ok1((sr = agar_dfs_new(NULL, (gr))));			\
		test_dfs_partial(sr, first, __VA_ARGS__);		\
		tal_free(sr);						\
	} while (0)

int main(void)
{
	struct parallel_graphr pgr;
	struct full_graphr fgr;
	struct chain_graphr cgr;
	struct grid_graphr ggr1, ggr2;
	struct agar_state *sr;
	const void *nr;
	
	plan_tests(2 * 13 + 12 + 10 + 6);

	test_dfs(&trivial_graphr.gr, 1, 1);

	parallel_graphr_init(&pgr, 3, 0);
	test_dfs(&pgr.gr, 1, 1, 2);

	full_graphr_init(&fgr, 5);
	test_dfs(&fgr.gr, 1, 1, 2, 3, 4, 5);
	test_dfs(&fgr.gr, 3, 3, 1, 2, 4, 5);

	chain_graphr_init(&cgr, 8);
	test_dfs(&cgr.fgr.gr, 1, 1, 2, 3, 4, 5, 6, 7, 8);
	test_dfs(&cgr.fgr.gr, 8, 8, 7, 6, 5, 4, 3, 2, 1);
	test_dfs(&cgr.fgr.gr, 5, 5, 4, 3, 2, 1, 6, 7, 8);

	grid_graphr_init(&ggr1, 3, 3, true, true, false, false);
	test_dfs(&ggr1.gr, 1, 1, 2, 3, 6, 9, 5, 8, 4, 7);
	test_dfs(&ggr1.gr, 5, 5, 6, 9, 8);
	test_dfs(&ggr1.gr, 9, 9);

	grid_graphr_init(&ggr2, 3, 3, true, true, true, true);
	test_dfs(&ggr2.gr, 1, 1, 2, 3, 6, 9, 8, 7, 4, 5);
	test_dfs(&ggr2.gr, 5, 5, 6, 9, 8, 7, 4, 1, 2, 3);
	test_dfs(&ggr2.gr, 9, 9, 8, 7, 4, 5, 6, 3, 2, 1);

	test_dfs(&error_graphr.gr, 1, 1, 2);
	ok((sr = agar_dfs_new(NULL, &error_graphr.gr)),
	   "started error traversal");
	ok1(agar_dfs_explore(sr, int2ptr(3), &nr));
	ok(ptr2int(nr) == 3, "Expected node #3, actually #%ld", ptr2int(nr));
	ok1(agar_dfs_explore(sr, nr, &nr));
	ok(ptr2int(nr) == 4, "Expected node #4, actually #%ld", ptr2int(nr));
	ok1(!agar_dfs_explore(sr, nr, &nr));
	ok(agar_error(sr) == -1, "Error is %d (expected -1)", agar_error(sr));
	ok1(!agar_dfs_explore(sr, nr, &nr));
	tal_free(sr);
	test_dfs(&error_graphr.gr, 1, 1, 2);

	test_dfs(&traversal1_graphr.gr, 1, 1, 2, 4, 5, 3, 6);
	test_dfs(&traversal1_graphr.gr, 9, 9, 8, 6, 5, 7, 4);

	ok1((sr = agar_dfs_new(NULL, &traversal1_graphr.gr)));
	test_dfs_partial(sr, 1, 1, 2, 4, 5, 3, 6);
	test_dfs_partial(sr, 9, 9, 8, 7);
	tal_free(sr);

	ok1((sr = agar_dfs_new(NULL, &traversal1_graphr.gr)));
	test_dfs_partial(sr, 9, 9, 8, 6, 5, 7, 4);
	test_dfs_partial(sr, 1, 1, 2, 3);
	tal_free(sr);

	test_dfs(&negacycle_graphr.gr, 1, 1, 2, 3);
	test_dfs(&negacycle_graphr.gr, 2, 2, 3, 1);
	test_dfs(&negacycle_graphr.gr, 3, 3, 1, 2);

	return exit_status();
}
