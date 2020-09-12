endian_ok_deps := ccan/endian/test/compile_ok-constant.c ccan/endian/test/run.c   
ccan/endian/.ok: $(endian_ok_deps)
ccan/endian/.fast-ok: $(endian_ok_deps:%.ok=%.fast-ok)
