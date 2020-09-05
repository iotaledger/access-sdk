crcsync_ok_deps := ccan/crcsync/test/run.c ccan/crcsync/test/run-crash.c ccan/crcsync/test/run-crc.c ccan/crcsync/test/run-roll.c  ccan/crcsync/crcsync.o  ccan/crc/.ok 
ccan/crcsync/.ok: $(crcsync_ok_deps)
ccan/crcsync/.fast-ok: $(crcsync_ok_deps:%.ok=%.fast-ok)
