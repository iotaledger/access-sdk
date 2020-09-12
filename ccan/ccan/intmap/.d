intmap_ok_deps := ccan/intmap/test/run.c ccan/intmap/test/run-order.c ccan/intmap/test/run-order-smallsize.c ccan/intmap/test/run-signed-int.c  ccan/intmap/intmap.o  ccan/ilog/.ok ccan/short_types/.ok ccan/str/.ok ccan/tcon/.ok ccan/typesafe_cb/.ok 
ccan/intmap/.ok: $(intmap_ok_deps)
ccan/intmap/.fast-ok: $(intmap_ok_deps:%.ok=%.fast-ok)
