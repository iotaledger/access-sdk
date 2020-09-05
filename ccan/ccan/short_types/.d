short_types_ok_deps := ccan/short_types/test/run.c ccan/short_types/test/run-endian.c   
ccan/short_types/.ok: $(short_types_ok_deps)
ccan/short_types/.fast-ok: $(short_types_ok_deps:%.ok=%.fast-ok)
