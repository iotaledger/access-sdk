str/hex_ok_deps := ccan/str/hex/test/run.c  ccan/str/hex/hex.o  
ccan/str/hex/.ok: $(str/hex_ok_deps)
ccan/str/hex/.fast-ok: $(str/hex_ok_deps:%.ok=%.fast-ok)
