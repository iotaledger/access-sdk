daemon_with_notify_ok_deps := ccan/daemon_with_notify/test/run.c  ccan/daemon_with_notify/daemon_with_notify.o  
ccan/daemon_with_notify/.ok: $(daemon_with_notify_ok_deps)
ccan/daemon_with_notify/.fast-ok: $(daemon_with_notify_ok_deps:%.ok=%.fast-ok)
