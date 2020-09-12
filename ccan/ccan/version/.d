version_ok_deps := ccan/version/test/compile_fail.c ccan/version/test/run.c   
ccan/version/.ok: $(version_ok_deps)
ccan/version/.fast-ok: $(version_ok_deps:%.ok=%.fast-ok)
