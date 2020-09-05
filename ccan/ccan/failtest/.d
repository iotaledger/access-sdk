failtest_ok_deps := ccan/failtest/test/run-failpath.c ccan/failtest/test/run-history.c ccan/failtest/test/run-locking.c ccan/failtest/test/run-malloc.c ccan/failtest/test/run-open.c ccan/failtest/test/run-with-fdlimit.c ccan/failtest/test/run-write.c  ccan/failtest/failtest.o  ccan/build_assert/.ok ccan/compiler/.ok ccan/err/.ok ccan/hash/.ok ccan/htable/.ok ccan/read_write_all/.ok ccan/str/.ok ccan/time/.ok ccan/tlist/.ok 
ccan/failtest/.ok: $(failtest_ok_deps)
ccan/failtest/.fast-ok: $(failtest_ok_deps:%.ok=%.fast-ok)
