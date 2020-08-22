take_ok_deps := ccan/take/test/run.c ccan/take/test/run-debug.c  ccan/take/take.o  ccan/likely/.ok ccan/str/.ok 
ccan/take/.ok: $(take_ok_deps)
ccan/take/.fast-ok: $(take_ok_deps:%.ok=%.fast-ok)
