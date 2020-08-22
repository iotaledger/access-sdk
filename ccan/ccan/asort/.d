asort_ok_deps := ccan/asort/test/compile_fail-context-type.c ccan/asort/test/run.c  ccan/asort/asort.o  ccan/order/.ok 
ccan/asort/.ok: $(asort_ok_deps)
ccan/asort/.fast-ok: $(asort_ok_deps:%.ok=%.fast-ok)
