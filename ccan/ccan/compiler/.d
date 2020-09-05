compiler_ok_deps := ccan/compiler/test/compile_fail-printf.c ccan/compiler/test/run-is_compile_constant.c   
ccan/compiler/.ok: $(compiler_ok_deps)
ccan/compiler/.fast-ok: $(compiler_ok_deps:%.ok=%.fast-ok)
