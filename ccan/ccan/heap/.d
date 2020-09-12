heap_ok_deps := ccan/heap/test/run.c  ccan/heap/heap.o  
ccan/heap/.ok: $(heap_ok_deps)
ccan/heap/.fast-ok: $(heap_ok_deps:%.ok=%.fast-ok)
