jacobson_karels_ok_deps := ccan/jacobson_karels/test/run.c ccan/jacobson_karels/test/run-nouse.c   ccan/minmax/.ok 
ccan/jacobson_karels/.ok: $(jacobson_karels_ok_deps)
ccan/jacobson_karels/.fast-ok: $(jacobson_karels_ok_deps:%.ok=%.fast-ok)
