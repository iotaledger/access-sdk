minmax_ok_deps := ccan/minmax/test/compile_fail-wrongsign.c ccan/minmax/test/compile_fail-wrongsize.c ccan/minmax/test/run.c   ccan/build_assert/.ok 
ccan/minmax/.ok: $(minmax_ok_deps)
ccan/minmax/.fast-ok: $(minmax_ok_deps:%.ok=%.fast-ok)
