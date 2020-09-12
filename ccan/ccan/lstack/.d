lstack_ok_deps := ccan/lstack/test/run.c   ccan/tcon/.ok 
ccan/lstack/.ok: $(lstack_ok_deps)
ccan/lstack/.fast-ok: $(lstack_ok_deps:%.ok=%.fast-ok)
