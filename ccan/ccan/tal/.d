tal_ok_deps := ccan/tal/test/run-allocfail.c ccan/tal/test/run-array.c ccan/tal/test/run.c ccan/tal/test/run-count.c ccan/tal/test/run-destructor2.c ccan/tal/test/run-destructor.c ccan/tal/test/run-expand.c ccan/tal/test/run-free.c ccan/tal/test/run-groups-grow.c ccan/tal/test/run-iter.c ccan/tal/test/run-named.c ccan/tal/test/run-named-debug.c ccan/tal/test/run-named-nolabels.c ccan/tal/test/run-notifier.c ccan/tal/test/run-overflow.c ccan/tal/test/run-resizez.c ccan/tal/test/run-steal.c ccan/tal/test/run-take.c ccan/tal/test/run-test-backend.c  ccan/tal/tal.o  ccan/alignof/.ok ccan/compiler/.ok ccan/likely/.ok ccan/list/.ok ccan/str/.ok ccan/take/.ok ccan/typesafe_cb/.ok 
ccan/tal/.ok: $(tal_ok_deps)
ccan/tal/.fast-ok: $(tal_ok_deps:%.ok=%.fast-ok)
