crypto/hkdf_sha256_ok_deps := ccan/crypto/hkdf_sha256/test/api-rfc5869.c  ccan/crypto/hkdf_sha256/hkdf_sha256.o  ccan/crypto/hmac_sha256/.ok 
ccan/crypto/hkdf_sha256/.ok: $(crypto/hkdf_sha256_ok_deps)
ccan/crypto/hkdf_sha256/.fast-ok: $(crypto/hkdf_sha256_ok_deps:%.ok=%.fast-ok)
