talloc_ok_deps := ccan/talloc/test/compile_fail-talloc_set.c ccan/talloc/test/run.c ccan/talloc/test/run-corruption1.c ccan/talloc/test/run-external-alloc.c ccan/talloc/test/run-set_allocator.c ccan/talloc/test/run-talloc_set.c ccan/talloc/test/run-test_autofree.c ccan/talloc/test/run-test_free_in_destructor.c ccan/talloc/test/run-test_free_parent_deny_child.c ccan/talloc/test/run-test_loop.c ccan/talloc/test/run-test_misc.c ccan/talloc/test/run-test_move.c ccan/talloc/test/run-test_realloc.c ccan/talloc/test/run-test_realloc_fn.c ccan/talloc/test/run-test_ref.c ccan/talloc/test/run-test_steal.c ccan/talloc/test/run-test_talloc_ptrtype.c ccan/talloc/test/run-test_type.c ccan/talloc/test/run-test_unlink.c ccan/talloc/test/run-test_unref_reparent.c  ccan/talloc/talloc.o  ccan/compiler/.ok ccan/typesafe_cb/.ok 
ccan/talloc/.ok: $(talloc_ok_deps)
ccan/talloc/.fast-ok: $(talloc_ok_deps:%.ok=%.fast-ok)
