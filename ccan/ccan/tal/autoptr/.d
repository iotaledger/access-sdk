tal/autoptr_ok_deps := ccan/tal/autoptr/test/run.c  ccan/tal/autoptr/autoptr.o  ccan/tal/.ok 
ccan/tal/autoptr/.ok: $(tal/autoptr_ok_deps)
ccan/tal/autoptr/.fast-ok: $(tal/autoptr_ok_deps:%.ok=%.fast-ok)
