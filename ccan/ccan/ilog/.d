ilog_ok_deps := ccan/ilog/test/run.c ccan/ilog/test/run-out-of-line.c  ccan/ilog/ilog.o  ccan/compiler/.ok 
ccan/ilog/.ok: $(ilog_ok_deps)
ccan/ilog/.fast-ok: $(ilog_ok_deps:%.ok=%.fast-ok)
