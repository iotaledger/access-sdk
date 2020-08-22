crypto/ripemd160_ok_deps := ccan/crypto/ripemd160/test/run-lotsa-data.c ccan/crypto/ripemd160/test/run-test-vectors.c ccan/crypto/ripemd160/test/run-types.c  ccan/crypto/ripemd160/ripemd160.o  ccan/compiler/.ok ccan/endian/.ok 
ccan/crypto/ripemd160/.ok: $(crypto/ripemd160_ok_deps)
ccan/crypto/ripemd160/.fast-ok: $(crypto/ripemd160_ok_deps:%.ok=%.fast-ok)
