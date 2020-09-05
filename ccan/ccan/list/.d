list_ok_deps := ccan/list/test/compile_ok-constant.c ccan/list/test/helper.c ccan/list/test/helper.h ccan/list/test/run.c ccan/list/test/run-CCAN_LIST_DEBUG.c ccan/list/test/run-check-corrupt.c ccan/list/test/run-check-nonconst.c ccan/list/test/run-list_del_from-assert.c ccan/list/test/run-list_prev-list_next.c ccan/list/test/run-prepend_list.c ccan/list/test/run-single-eval.c ccan/list/test/run-with-debug.c  ccan/list/list.o  ccan/check_type/.ok ccan/container_of/.ok ccan/str/.ok 
ccan/list/.ok: $(list_ok_deps)
ccan/list/.fast-ok: $(list_ok_deps:%.ok=%.fast-ok)
