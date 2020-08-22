err_ok_deps := ccan/err/test/run.c  ccan/err/err.o  
ccan/err/.ok: $(err_ok_deps)
ccan/err/.fast-ok: $(err_ok_deps:%.ok=%.fast-ok)
