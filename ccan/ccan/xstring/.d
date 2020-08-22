xstring_ok_deps := ccan/xstring/test/run2.c ccan/xstring/test/run.c  ccan/xstring/xstring.o  
ccan/xstring/.ok: $(xstring_ok_deps)
ccan/xstring/.fast-ok: $(xstring_ok_deps:%.ok=%.fast-ok)
