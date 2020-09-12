antithread/alloc_ok_deps := ccan/antithread/alloc/test/run.c ccan/antithread/alloc/test/run-corrupt.c ccan/antithread/alloc/test/run-testsize.c ccan/antithread/alloc/test/run-tiny-encode.c  ccan/antithread/alloc/alloc.o ccan/antithread/alloc/bitops.o ccan/antithread/alloc/tiny.o  ccan/alignof/.ok ccan/build_assert/.ok ccan/compiler/.ok ccan/ilog/.ok ccan/likely/.ok ccan/short_types/.ok 
ccan/antithread/alloc/.ok: $(antithread/alloc_ok_deps)
ccan/antithread/alloc/.fast-ok: $(antithread/alloc_ok_deps:%.ok=%.fast-ok)
