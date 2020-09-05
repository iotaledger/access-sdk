isaac_ok_deps := ccan/isaac/test/run64.c ccan/isaac/test/run.c  ccan/isaac/isaac64.o ccan/isaac/isaac.o  ccan/ilog/.ok 
ccan/isaac/.ok: $(isaac_ok_deps)
ccan/isaac/.fast-ok: $(isaac_ok_deps:%.ok=%.fast-ok)
