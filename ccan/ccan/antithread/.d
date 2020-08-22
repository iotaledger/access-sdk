antithread_ok_deps := ccan/antithread/test/run-lock.c ccan/antithread/test/run-simple.c ccan/antithread/test/run-spawn.c ccan/antithread/test/run-spawn-NULL.c ccan/antithread/test/run-tell.c ccan/antithread/test/run-tell_parent.c  ccan/antithread/antithread.o  ccan/antithread/alloc/.ok ccan/err/.ok ccan/list/.ok ccan/noerr/.ok ccan/read_write_all/.ok ccan/talloc/.ok ccan/typesafe_cb/.ok 
ccan/antithread/.ok: $(antithread_ok_deps)
ccan/antithread/.fast-ok: $(antithread_ok_deps:%.ok=%.fast-ok)
