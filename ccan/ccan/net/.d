net_ok_deps := ccan/net/test/run-bind.c ccan/net/test/run.c  ccan/net/net.o  ccan/noerr/.ok 
ccan/net/.ok: $(net_ok_deps)
ccan/net/.fast-ok: $(net_ok_deps:%.ok=%.fast-ok)
