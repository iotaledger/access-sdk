strmap_ok_deps := ccan/strmap/test/run.c ccan/strmap/test/run-iterate-const.c ccan/strmap/test/run-order.c ccan/strmap/test/run-prefix.c  ccan/strmap/strmap.o  ccan/ilog/.ok ccan/short_types/.ok ccan/str/.ok ccan/tcon/.ok ccan/typesafe_cb/.ok 
ccan/strmap/.ok: $(strmap_ok_deps)
ccan/strmap/.fast-ok: $(strmap_ok_deps:%.ok=%.fast-ok)
