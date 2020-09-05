argcheck_ok_deps := ccan/argcheck/test/run.c   ccan/compiler/.ok ccan/likely/.ok 
ccan/argcheck/.ok: $(argcheck_ok_deps)
ccan/argcheck/.fast-ok: $(argcheck_ok_deps:%.ok=%.fast-ok)
