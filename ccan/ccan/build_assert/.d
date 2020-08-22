build_assert_ok_deps := ccan/build_assert/test/compile_fail.c ccan/build_assert/test/compile_fail-expr.c ccan/build_assert/test/compile_ok.c ccan/build_assert/test/run-BUILD_ASSERT_OR_ZERO.c   
ccan/build_assert/.ok: $(build_assert_ok_deps)
ccan/build_assert/.fast-ok: $(build_assert_ok_deps:%.ok=%.fast-ok)
