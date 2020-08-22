lbalance_ok_deps := ccan/lbalance/test/run.c  ccan/lbalance/lbalance.o  ccan/tlist2/.ok 
ccan/lbalance/.ok: $(lbalance_ok_deps)
ccan/lbalance/.fast-ok: $(lbalance_ok_deps:%.ok=%.fast-ok)
