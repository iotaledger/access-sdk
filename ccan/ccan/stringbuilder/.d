stringbuilder_ok_deps := ccan/stringbuilder/test/run.c  ccan/stringbuilder/stringbuilder.o  
ccan/stringbuilder/.ok: $(stringbuilder_ok_deps)
ccan/stringbuilder/.fast-ok: $(stringbuilder_ok_deps:%.ok=%.fast-ok)
