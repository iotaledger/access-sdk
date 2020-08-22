cppmagic_ok_deps := ccan/cppmagic/test/run.c   
ccan/cppmagic/.ok: $(cppmagic_ok_deps)
ccan/cppmagic/.fast-ok: $(cppmagic_ok_deps:%.ok=%.fast-ok)
