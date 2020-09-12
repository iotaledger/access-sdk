io/fdpass_ok_deps := ccan/io/fdpass/test/run.c  ccan/io/fdpass/fdpass.o  ccan/fdpass/.ok ccan/io/.ok 
ccan/io/fdpass/.ok: $(io/fdpass_ok_deps)
ccan/io/fdpass/.fast-ok: $(io/fdpass_ok_deps:%.ok=%.fast-ok)
