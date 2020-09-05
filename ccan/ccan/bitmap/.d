bitmap_ok_deps := ccan/bitmap/test/run-alloc.c ccan/bitmap/test/run.c ccan/bitmap/test/run-ffs.c ccan/bitmap/test/run-ranges.c  ccan/bitmap/bitmap.o  ccan/endian/.ok 
ccan/bitmap/.ok: $(bitmap_ok_deps)
ccan/bitmap/.fast-ok: $(bitmap_ok_deps:%.ok=%.fast-ok)
