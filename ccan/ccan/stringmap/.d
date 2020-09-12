stringmap_ok_deps := ccan/stringmap/test/run.c  ccan/stringmap/stringmap.o  ccan/block_pool/.ok 
ccan/stringmap/.ok: $(stringmap_ok_deps)
ccan/stringmap/.fast-ok: $(stringmap_ok_deps:%.ok=%.fast-ok)
