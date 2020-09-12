crypto/hmac_sha256_ok_deps := ccan/crypto/hmac_sha256/test/api-rfc4231.c  ccan/crypto/hmac_sha256/hmac_sha256.o  ccan/crypto/sha256/.ok 
ccan/crypto/hmac_sha256/.ok: $(crypto/hmac_sha256_ok_deps)
ccan/crypto/hmac_sha256/.fast-ok: $(crypto/hmac_sha256_ok_deps:%.ok=%.fast-ok)
