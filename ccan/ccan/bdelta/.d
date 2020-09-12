bdelta_ok_deps := ccan/bdelta/test/common.h ccan/bdelta/test/run-medium.c ccan/bdelta/test/run-myers.c ccan/bdelta/test/run-trivial.c  ccan/bdelta/bdelta.o  
ccan/bdelta/.ok: $(bdelta_ok_deps)
ccan/bdelta/.fast-ok: $(bdelta_ok_deps:%.ok=%.fast-ok)
