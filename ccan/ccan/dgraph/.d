dgraph_ok_deps := ccan/dgraph/test/run.c ccan/dgraph/test/run-debug.c  ccan/dgraph/dgraph.o  ccan/agar/.ok ccan/tlist/.ok ccan/typesafe_cb/.ok 
ccan/dgraph/.ok: $(dgraph_ok_deps)
ccan/dgraph/.fast-ok: $(dgraph_ok_deps:%.ok=%.fast-ok)
