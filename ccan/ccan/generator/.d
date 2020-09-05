generator_ok_deps := ccan/generator/test/api.c ccan/generator/test/compile_fail-1.c ccan/generator/test/compile_fail-2.c ccan/generator/test/compile_fail-3.c ccan/generator/test/compile_fail-4.c ccan/generator/test/compile_fail-5.c ccan/generator/test/example-gens.c ccan/generator/test/example-gens.h  ccan/generator/generator.o  ccan/alignof/.ok ccan/compiler/.ok ccan/coroutine/.ok ccan/cppmagic/.ok 
ccan/generator/.ok: $(generator_ok_deps)
ccan/generator/.fast-ok: $(generator_ok_deps:%.ok=%.fast-ok)
