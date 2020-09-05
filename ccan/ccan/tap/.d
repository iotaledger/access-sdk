tap_ok_deps := ccan/tap/test/run.c  ccan/tap/tap.o  ccan/compiler/.ok 
ccan/tap/.ok: $(tap_ok_deps)
ccan/tap/.fast-ok: $(tap_ok_deps:%.ok=%.fast-ok)
