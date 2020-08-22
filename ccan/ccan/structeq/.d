structeq_ok_deps := ccan/structeq/test/compile_fail-different.c ccan/structeq/test/run.c   
ccan/structeq/.ok: $(structeq_ok_deps)
ccan/structeq/.fast-ok: $(structeq_ok_deps:%.ok=%.fast-ok)
