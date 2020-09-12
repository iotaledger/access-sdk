lpq_ok_deps := ccan/lpq/test/api.c  ccan/lpq/lpq.o  ccan/cast/.ok ccan/order/.ok ccan/tcon/.ok 
ccan/lpq/.ok: $(lpq_ok_deps)
ccan/lpq/.fast-ok: $(lpq_ok_deps:%.ok=%.fast-ok)
