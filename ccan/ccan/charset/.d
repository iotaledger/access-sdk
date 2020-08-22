charset_ok_deps := ccan/charset/test/common.h ccan/charset/test/run-surrogate-pair.c ccan/charset/test/run-utf8-read-write.c ccan/charset/test/run-utf8_validate.c  ccan/charset/charset.o  
ccan/charset/.ok: $(charset_ok_deps)
ccan/charset/.fast-ok: $(charset_ok_deps:%.ok=%.fast-ok)
