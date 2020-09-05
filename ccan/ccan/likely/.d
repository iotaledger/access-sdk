likely_ok_deps := ccan/likely/test/run.c ccan/likely/test/run-debug.c  ccan/likely/likely.o  
ccan/likely/.ok: $(likely_ok_deps)
ccan/likely/.fast-ok: $(likely_ok_deps:%.ok=%.fast-ok)
