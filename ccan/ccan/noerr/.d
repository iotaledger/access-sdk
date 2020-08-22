noerr_ok_deps := ccan/noerr/test/run.c  ccan/noerr/noerr.o  
ccan/noerr/.ok: $(noerr_ok_deps)
ccan/noerr/.fast-ok: $(noerr_ok_deps:%.ok=%.fast-ok)
