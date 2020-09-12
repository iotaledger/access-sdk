objset_ok_deps := ccan/objset/test/run.c   ccan/hash/.ok ccan/htable/.ok ccan/tcon/.ok 
ccan/objset/.ok: $(objset_ok_deps)
ccan/objset/.fast-ok: $(objset_ok_deps:%.ok=%.fast-ok)
