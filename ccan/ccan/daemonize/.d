daemonize_ok_deps := ccan/daemonize/test/run.c  ccan/daemonize/daemonize.o  
ccan/daemonize/.ok: $(daemonize_ok_deps)
ccan/daemonize/.fast-ok: $(daemonize_ok_deps:%.ok=%.fast-ok)
