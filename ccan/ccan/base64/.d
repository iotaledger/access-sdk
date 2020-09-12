base64_ok_deps := ccan/base64/test/moretap.h ccan/base64/test/run.c  ccan/base64/base64.o  
ccan/base64/.ok: $(base64_ok_deps)
ccan/base64/.fast-ok: $(base64_ok_deps:%.ok=%.fast-ok)
