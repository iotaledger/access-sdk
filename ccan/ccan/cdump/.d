cdump_ok_deps := ccan/cdump/test/run-arraysize.c ccan/cdump/test/run-attributes.c ccan/cdump/test/run.c ccan/cdump/test/run-CDUMP.c ccan/cdump/test/run-enum-comma.c ccan/cdump/test/run-forward-decl.c ccan/cdump/test/run-multiline.c ccan/cdump/test/run-qualifiers.c  ccan/cdump/cdump.o  ccan/strmap/.ok ccan/tal/.ok ccan/tal/str/.ok 
ccan/cdump/.ok: $(cdump_ok_deps)
ccan/cdump/.fast-ok: $(cdump_ok_deps:%.ok=%.fast-ok)
