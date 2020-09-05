crypto/sha256_ok_deps := ccan/crypto/sha256/test/run-33-bit-test.c ccan/crypto/sha256/test/run-lotsa-data.c ccan/crypto/sha256/test/run-test-vectors.c ccan/crypto/sha256/test/run-types.c  ccan/crypto/sha256/sha256.o  ccan/compiler/.ok ccan/endian/.ok 
ccan/crypto/sha256/.ok: $(crypto/sha256_ok_deps)
ccan/crypto/sha256/.fast-ok: $(crypto/sha256_ok_deps:%.ok=%.fast-ok)
