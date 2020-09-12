tal/link_ok_deps := ccan/tal/link/test/run.c  ccan/tal/link/link.o  ccan/container_of/.ok ccan/list/.ok ccan/tal/.ok 
ccan/tal/link/.ok: $(tal/link_ok_deps)
ccan/tal/link/.fast-ok: $(tal/link_ok_deps:%.ok=%.fast-ok)
