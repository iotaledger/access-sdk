pipecmd_ok_deps := ccan/pipecmd/test/run.c ccan/pipecmd/test/run-fdleak.c  ccan/pipecmd/pipecmd.o  ccan/noerr/.ok 
ccan/pipecmd/.ok: $(pipecmd_ok_deps)
ccan/pipecmd/.fast-ok: $(pipecmd_ok_deps:%.ok=%.fast-ok)
