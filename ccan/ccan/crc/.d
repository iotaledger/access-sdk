crc_ok_deps := ccan/crc/test/api.c  ccan/crc/crc.o  ccan/array_size/.ok 
ccan/crc/.ok: $(crc_ok_deps)
ccan/crc/.fast-ok: $(crc_ok_deps:%.ok=%.fast-ok)
