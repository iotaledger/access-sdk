cast_ok_deps := ccan/cast/test/compile_fail-cast_const2.c ccan/cast/test/compile_fail-cast_const3.c ccan/cast/test/compile_fail-cast_const.c ccan/cast/test/compile_fail-cast_signed.c ccan/cast/test/compile_fail-cast_signed-const.c ccan/cast/test/compile_fail-cast_signed-sizesame.c ccan/cast/test/compile_fail-cast_static-2.c ccan/cast/test/compile_fail-cast_static-3.c ccan/cast/test/compile_fail-cast_static.c ccan/cast/test/compile_ok-cast_void.c ccan/cast/test/compile_ok-static.c   ccan/build_assert/.ok 
ccan/cast/.ok: $(cast_ok_deps)
ccan/cast/.fast-ok: $(cast_ok_deps:%.ok=%.fast-ok)
