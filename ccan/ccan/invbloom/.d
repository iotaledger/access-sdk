invbloom_ok_deps := ccan/invbloom/test/run.c ccan/invbloom/test/run-singletoncb.c ccan/invbloom/test/run-subtract.c  ccan/invbloom/invbloom.o  ccan/endian/.ok ccan/hash/.ok ccan/short_types/.ok ccan/tal/.ok ccan/typesafe_cb/.ok 
ccan/invbloom/.ok: $(invbloom_ok_deps)
ccan/invbloom/.fast-ok: $(invbloom_ok_deps:%.ok=%.fast-ok)
