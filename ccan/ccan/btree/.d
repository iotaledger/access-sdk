btree_ok_deps := ccan/btree/test/run-benchmark.c ccan/btree/test/run-random-access.c ccan/btree/test/run-search-implement.c ccan/btree/test/run-trivial.c  ccan/btree/btree.o  
ccan/btree/.ok: $(btree_ok_deps)
ccan/btree/.fast-ok: $(btree_ok_deps:%.ok=%.fast-ok)
