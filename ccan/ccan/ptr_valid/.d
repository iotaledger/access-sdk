ptr_valid_ok_deps := ccan/ptr_valid/test/run.c ccan/ptr_valid/test/run-string.c  ccan/ptr_valid/ptr_valid.o  ccan/noerr/.ok 
ccan/ptr_valid/.ok: $(ptr_valid_ok_deps)
ccan/ptr_valid/.fast-ok: $(ptr_valid_ok_deps:%.ok=%.fast-ok)
