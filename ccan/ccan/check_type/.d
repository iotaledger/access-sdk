check_type_ok_deps := ccan/check_type/test/compile_fail-check_type.c ccan/check_type/test/compile_fail-check_types_match.c ccan/check_type/test/compile_fail-check_type_unsigned.c ccan/check_type/test/run.c   
ccan/check_type/.ok: $(check_type_ok_deps)
ccan/check_type/.fast-ok: $(check_type_ok_deps:%.ok=%.fast-ok)
