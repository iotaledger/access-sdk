rszshm_ok_deps := ccan/rszshm/test/run.c  ccan/rszshm/rszshm.o  
ccan/rszshm/.ok: $(rszshm_ok_deps)
ccan/rszshm/.fast-ok: $(rszshm_ok_deps:%.ok=%.fast-ok)
