time_ok_deps := ccan/time/test/run.c ccan/time/test/run-check.c ccan/time/test/run-monotonic.c  ccan/time/time.o  
ccan/time/.ok: $(time_ok_deps)
ccan/time/.fast-ok: $(time_ok_deps:%.ok=%.fast-ok)
