pushpull_ok_deps := ccan/pushpull/test/run.c  ccan/pushpull/pull.o ccan/pushpull/push.o  ccan/endian/.ok 
ccan/pushpull/.ok: $(pushpull_ok_deps)
ccan/pushpull/.fast-ok: $(pushpull_ok_deps:%.ok=%.fast-ok)
