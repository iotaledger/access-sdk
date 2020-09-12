lqueue_ok_deps := ccan/lqueue/test/run.c   ccan/tcon/.ok 
ccan/lqueue/.ok: $(lqueue_ok_deps)
ccan/lqueue/.fast-ok: $(lqueue_ok_deps:%.ok=%.fast-ok)
