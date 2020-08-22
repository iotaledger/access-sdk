json_ok_deps := ccan/json/test/common.h ccan/json/test/run-construction.c ccan/json/test/run-decode-encode.c ccan/json/test/run-stringify.c ccan/json/test/run-validate.c  ccan/json/json.o  
ccan/json/.ok: $(json_ok_deps)
ccan/json/.fast-ok: $(json_ok_deps:%.ok=%.fast-ok)
