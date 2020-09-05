eratosthenes_ok_deps := ccan/eratosthenes/test/run.c ccan/eratosthenes/test/run-incremental.c  ccan/eratosthenes/eratosthenes.o  ccan/bitmap/.ok 
ccan/eratosthenes/.ok: $(eratosthenes_ok_deps)
ccan/eratosthenes/.fast-ok: $(eratosthenes_ok_deps:%.ok=%.fast-ok)
