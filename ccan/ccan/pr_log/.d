pr_log_ok_deps := ccan/pr_log/test/run.c ccan/pr_log/test/run-debug.c ccan/pr_log/test/run-disable.c  ccan/pr_log/pr_log.o  ccan/compiler/.ok ccan/str/.ok 
ccan/pr_log/.ok: $(pr_log_ok_deps)
ccan/pr_log/.fast-ok: $(pr_log_ok_deps:%.ok=%.fast-ok)
