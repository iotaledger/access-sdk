tlist2_ok_deps := ccan/tlist2/test/compile_fail-tlist_add_2.c ccan/tlist2/test/compile_fail-tlist_add_tail_2.c ccan/tlist2/test/compile_fail-tlist_del_from_2.c ccan/tlist2/test/compile_fail-tlist_for_each_2.c ccan/tlist2/test/compile_fail-tlist_for_each_safe_2.c ccan/tlist2/test/compile_fail-tlist_tail_2.c ccan/tlist2/test/compile_fail-tlist_top_2.c ccan/tlist2/test/run_2.c   ccan/list/.ok ccan/tcon/.ok 
ccan/tlist2/.ok: $(tlist2_ok_deps)
ccan/tlist2/.fast-ok: $(tlist2_ok_deps:%.ok=%.fast-ok)
