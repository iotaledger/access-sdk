alignof_ok_deps := ccan/alignof/test/run.c   
ccan/alignof/.ok: $(alignof_ok_deps)
ccan/alignof/.fast-ok: $(alignof_ok_deps:%.ok=%.fast-ok)
