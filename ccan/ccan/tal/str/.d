tal/str_ok_deps := ccan/tal/str/test/helper.h ccan/tal/str/test/run.c ccan/tal/str/test/run-fmt-terminate.c ccan/tal/str/test/run-string.c ccan/tal/str/test/run-strndup.c ccan/tal/str/test/run-strreg.c ccan/tal/str/test/run-take.c  ccan/tal/str/str.o  ccan/str/.ok ccan/take/.ok ccan/tal/.ok 
ccan/tal/str/.ok: $(tal/str_ok_deps)
ccan/tal/str/.fast-ok: $(tal/str_ok_deps:%.ok=%.fast-ok)
