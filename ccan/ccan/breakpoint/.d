breakpoint_ok_deps := ccan/breakpoint/test/run.c  ccan/breakpoint/breakpoint.o  ccan/compiler/.ok 
ccan/breakpoint/.ok: $(breakpoint_ok_deps)
ccan/breakpoint/.fast-ok: $(breakpoint_ok_deps:%.ok=%.fast-ok)
