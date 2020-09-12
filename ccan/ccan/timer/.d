timer_ok_deps := ccan/timer/test/run-add.c ccan/timer/test/run.c ccan/timer/test/run-corrupt2.c ccan/timer/test/run-corrupt.c ccan/timer/test/run-expiry.c ccan/timer/test/run-ff.c ccan/timer/test/run-original-corrupt.c  ccan/timer/timer.o  ccan/array_size/.ok ccan/ilog/.ok ccan/likely/.ok ccan/list/.ok ccan/time/.ok 
ccan/timer/.ok: $(timer_ok_deps)
ccan/timer/.fast-ok: $(timer_ok_deps:%.ok=%.fast-ok)
