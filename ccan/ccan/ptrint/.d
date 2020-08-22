ptrint_ok_deps := ccan/ptrint/test/run.c   ccan/build_assert/.ok ccan/compiler/.ok 
ccan/ptrint/.ok: $(ptrint_ok_deps)
ccan/ptrint/.fast-ok: $(ptrint_ok_deps:%.ok=%.fast-ok)
