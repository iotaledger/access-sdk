container_of_ok_deps := ccan/container_of/test/compile_fail-bad-type.c ccan/container_of/test/compile_fail-types.c ccan/container_of/test/compile_fail-var-types.c ccan/container_of/test/run.c   ccan/check_type/.ok 
ccan/container_of/.ok: $(container_of_ok_deps)
ccan/container_of/.fast-ok: $(container_of_ok_deps:%.ok=%.fast-ok)
