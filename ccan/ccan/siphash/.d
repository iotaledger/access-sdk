siphash_ok_deps := ccan/siphash/test/api.c  ccan/siphash/siphash.o  ccan/endian/.ok 
ccan/siphash/.ok: $(siphash_ok_deps)
ccan/siphash/.fast-ok: $(siphash_ok_deps:%.ok=%.fast-ok)
