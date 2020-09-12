tal/grab_file_ok_deps := ccan/tal/grab_file/test/run-grab.c  ccan/tal/grab_file/grab_file.o  ccan/noerr/.ok ccan/tal/.ok 
ccan/tal/grab_file/.ok: $(tal/grab_file_ok_deps)
ccan/tal/grab_file/.fast-ok: $(tal/grab_file_ok_deps:%.ok=%.fast-ok)
