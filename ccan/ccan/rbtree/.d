rbtree_ok_deps := ccan/rbtree/test/run.c ccan/rbtree/test/run-many.c  ccan/rbtree/rbtree.o  ccan/failtest/.ok ccan/talloc/.ok 
ccan/rbtree/.ok: $(rbtree_ok_deps)
ccan/rbtree/.fast-ok: $(rbtree_ok_deps:%.ok=%.fast-ok)
