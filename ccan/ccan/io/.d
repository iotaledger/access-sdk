io_ok_deps := ccan/io/test/run-01-start-finish.c ccan/io/test/run-02-read.c ccan/io/test/run-03-readpartial.c ccan/io/test/run-04-writepartial.c ccan/io/test/run-05-write.c ccan/io/test/run-06-idle.c ccan/io/test/run-07-break.c ccan/io/test/run-08-hangup-on-idle.c ccan/io/test/run-08-read-after-hangup.c ccan/io/test/run-09-connect.c ccan/io/test/run-10-many.c ccan/io/test/run-12-bidir.c ccan/io/test/run-13-all-idle.c ccan/io/test/run-14-duplex-both-read.c ccan/io/test/run-15-timeout.c ccan/io/test/run-16-duplex-test.c ccan/io/test/run-17-homemade-io.c ccan/io/test/run-18-errno.c ccan/io/test/run-19-always.c ccan/io/test/run-20-io_time_override.c ccan/io/test/run-21-io_close_taken_fd.c ccan/io/test/run-22-POLLHUP-on-listening-socket.c ccan/io/test/run-30-io_flush_sync.c ccan/io/test/run-40-wakeup-mutual.c ccan/io/test/run-41-io_poll_override.c  ccan/io/io.o ccan/io/poll.o  ccan/container_of/.ok ccan/list/.ok ccan/tal/.ok ccan/time/.ok ccan/timer/.ok ccan/typesafe_cb/.ok 
ccan/io/.ok: $(io_ok_deps)
ccan/io/.fast-ok: $(io_ok_deps:%.ok=%.fast-ok)