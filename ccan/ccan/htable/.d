htable_ok_deps := ccan/htable/test/run.c ccan/htable/test/run-copy.c ccan/htable/test/run-size.c ccan/htable/test/run-type.c ccan/htable/test/run-type-int.c ccan/htable/test/run-zero-hash-first-entry.c  ccan/htable/htable.o  ccan/compiler/.ok 
ccan/htable/.ok: $(htable_ok_deps)
ccan/htable/.fast-ok: $(htable_ok_deps:%.ok=%.fast-ok)
