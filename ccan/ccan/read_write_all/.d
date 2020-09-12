read_write_all_ok_deps := ccan/read_write_all/test/run-read_all.c ccan/read_write_all/test/run-write_all.c  ccan/read_write_all/read_write_all.o  
ccan/read_write_all/.ok: $(read_write_all_ok_deps)
ccan/read_write_all/.fast-ok: $(read_write_all_ok_deps:%.ok=%.fast-ok)
