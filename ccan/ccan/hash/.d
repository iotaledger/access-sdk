hash_ok_deps := ccan/hash/test/api-hash_stable.c ccan/hash/test/run.c  ccan/hash/hash.o  ccan/build_assert/.ok 
ccan/hash/.ok: $(hash_ok_deps)
ccan/hash/.fast-ok: $(hash_ok_deps:%.ok=%.fast-ok)
