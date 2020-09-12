strset_ok_deps := ccan/strset/test/run.c ccan/strset/test/run-hibit.c ccan/strset/test/run-iterate-const.c ccan/strset/test/run-order.c ccan/strset/test/run-prefix.c  ccan/strset/strset.o  ccan/ilog/.ok ccan/likely/.ok ccan/short_types/.ok ccan/str/.ok ccan/typesafe_cb/.ok 
ccan/strset/.ok: $(strset_ok_deps)
ccan/strset/.fast-ok: $(strset_ok_deps:%.ok=%.fast-ok)
