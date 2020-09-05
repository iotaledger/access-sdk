mem_ok_deps := ccan/mem/test/api.c ccan/mem/test/api-memcheck.c ccan/mem/test/compile_fail-memcheck.c  ccan/mem/mem.o  ccan/compiler/.ok 
ccan/mem/.ok: $(mem_ok_deps)
ccan/mem/.fast-ok: $(mem_ok_deps:%.ok=%.fast-ok)
