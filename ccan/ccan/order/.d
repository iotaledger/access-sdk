order_ok_deps := ccan/order/test/api.c ccan/order/test/compile_fail_1.c ccan/order/test/compile_fail_2.c ccan/order/test/compile_ok.c ccan/order/test/fancy_cmp.h ccan/order/test/run-fancy.c  ccan/order/order.o  ccan/ptrint/.ok ccan/typesafe_cb/.ok 
ccan/order/.ok: $(order_ok_deps)
ccan/order/.fast-ok: $(order_ok_deps:%.ok=%.fast-ok)
