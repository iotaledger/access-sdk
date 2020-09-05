autodata_ok_deps := ccan/autodata/test/helper.c ccan/autodata/test/run.c ccan/autodata/test/run-fools.c  ccan/autodata/autodata.o  ccan/compiler/.ok ccan/ptr_valid/.ok 
ccan/autodata/.ok: $(autodata_ok_deps)
ccan/autodata/.fast-ok: $(autodata_ok_deps:%.ok=%.fast-ok)
