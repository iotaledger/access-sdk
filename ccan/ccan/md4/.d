md4_ok_deps := ccan/md4/test/api.c  ccan/md4/md4.o  ccan/array_size/.ok ccan/endian/.ok 
ccan/md4/.ok: $(md4_ok_deps)
ccan/md4/.fast-ok: $(md4_ok_deps:%.ok=%.fast-ok)
