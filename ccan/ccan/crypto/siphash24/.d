crypto/siphash24_ok_deps := ccan/crypto/siphash24/test/run.c  ccan/crypto/siphash24/siphash24.o  ccan/endian/.ok 
ccan/crypto/siphash24/.ok: $(crypto/siphash24_ok_deps)
ccan/crypto/siphash24/.fast-ok: $(crypto/siphash24_ok_deps:%.ok=%.fast-ok)
