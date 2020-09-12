fdpass_ok_deps := ccan/fdpass/test/run.c  ccan/fdpass/fdpass.o  
ccan/fdpass/.ok: $(fdpass_ok_deps)
ccan/fdpass/.fast-ok: $(fdpass_ok_deps:%.ok=%.fast-ok)
