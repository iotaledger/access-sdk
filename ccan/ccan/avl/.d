avl_ok_deps := ccan/avl/test/run.c  ccan/avl/avl.o  ccan/order/.ok 
ccan/avl/.ok: $(avl_ok_deps)
ccan/avl/.fast-ok: $(avl_ok_deps:%.ok=%.fast-ok)
