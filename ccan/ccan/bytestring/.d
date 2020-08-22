bytestring_ok_deps := ccan/bytestring/test/compile_fail-BYTESTRING-2.c ccan/bytestring/test/compile_fail-BYTESTRING.c ccan/bytestring/test/run.c  ccan/bytestring/bytestring.o  ccan/array_size/.ok ccan/compiler/.ok ccan/mem/.ok 
ccan/bytestring/.ok: $(bytestring_ok_deps)
ccan/bytestring/.fast-ok: $(bytestring_ok_deps:%.ok=%.fast-ok)
