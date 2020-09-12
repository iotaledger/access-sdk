asearch_ok_deps := ccan/asearch/test/compile_fail-return-value.c ccan/asearch/test/compile_fail-return-value-const.c ccan/asearch/test/run.c ccan/asearch/test/run-strings.c  ccan/asearch/asearch.o  ccan/typesafe_cb/.ok 
ccan/asearch/.ok: $(asearch_ok_deps)
ccan/asearch/.fast-ok: $(asearch_ok_deps:%.ok=%.fast-ok)
