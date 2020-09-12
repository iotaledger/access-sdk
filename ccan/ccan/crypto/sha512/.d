crypto/sha512_ok_deps := ccan/crypto/sha512/test/run-lotsa-data.c ccan/crypto/sha512/test/run-test-vectors.c  ccan/crypto/sha512/sha512.o  ccan/compiler/.ok ccan/endian/.ok 
ccan/crypto/sha512/.ok: $(crypto/sha512_ok_deps)
ccan/crypto/sha512/.fast-ok: $(crypto/sha512_ok_deps:%.ok=%.fast-ok)
