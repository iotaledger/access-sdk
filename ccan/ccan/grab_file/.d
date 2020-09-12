grab_file_ok_deps := ccan/grab_file/test/run-grab.c  ccan/grab_file/grab_file.o  ccan/noerr/.ok ccan/talloc/.ok 
ccan/grab_file/.ok: $(grab_file_ok_deps)
ccan/grab_file/.fast-ok: $(grab_file_ok_deps:%.ok=%.fast-ok)
