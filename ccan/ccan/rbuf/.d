rbuf_ok_deps := ccan/rbuf/test/run-all.c ccan/rbuf/test/run.c ccan/rbuf/test/run-open.c ccan/rbuf/test/run-partial-read.c ccan/rbuf/test/run-term-eof.c  ccan/rbuf/rbuf.o  
ccan/rbuf/.ok: $(rbuf_ok_deps)
ccan/rbuf/.fast-ok: $(rbuf_ok_deps:%.ok=%.fast-ok)
