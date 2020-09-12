asprintf_ok_deps := ccan/asprintf/test/run.c  ccan/asprintf/asprintf.o  ccan/compiler/.ok 
ccan/asprintf/.ok: $(asprintf_ok_deps)
ccan/asprintf/.fast-ok: $(asprintf_ok_deps:%.ok=%.fast-ok)
