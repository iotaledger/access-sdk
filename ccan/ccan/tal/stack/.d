tal/stack_ok_deps := ccan/tal/stack/test/run-stack.c  ccan/tal/stack/stack.o  ccan/tal/.ok 
ccan/tal/stack/.ok: $(tal/stack_ok_deps)
ccan/tal/stack/.fast-ok: $(tal/stack_ok_deps:%.ok=%.fast-ok)
