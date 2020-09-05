foreach_ok_deps := ccan/foreach/test/compile_ok-const.c ccan/foreach/test/compile_ok-nonconst.c ccan/foreach/test/run-break.c ccan/foreach/test/run.c ccan/foreach/test/run-nested.c ccan/foreach/test/run-not-on-stack.c ccan/foreach/test/run-single-arg.c  ccan/foreach/foreach.o  
ccan/foreach/.ok: $(foreach_ok_deps)
ccan/foreach/.fast-ok: $(foreach_ok_deps:%.ok=%.fast-ok)
