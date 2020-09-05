sparse_bsearch_ok_deps := ccan/sparse_bsearch/test/run.c  ccan/sparse_bsearch/sparse_bsearch.o  ccan/check_type/.ok ccan/typesafe_cb/.ok 
ccan/sparse_bsearch/.ok: $(sparse_bsearch_ok_deps)
ccan/sparse_bsearch/.fast-ok: $(sparse_bsearch_ok_deps:%.ok=%.fast-ok)
