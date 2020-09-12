crypto/shachain_ok_deps := ccan/crypto/shachain/test/run-8bit.c ccan/crypto/shachain/test/run-badhash.c ccan/crypto/shachain/test/run.c ccan/crypto/shachain/test/run-can_derive.c  ccan/crypto/shachain/shachain.o  ccan/crypto/sha256/.ok ccan/ilog/.ok 
ccan/crypto/shachain/.ok: $(crypto/shachain_ok_deps)
ccan/crypto/shachain/.fast-ok: $(crypto/shachain_ok_deps:%.ok=%.fast-ok)
