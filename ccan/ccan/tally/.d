tally_ok_deps := ccan/tally/test/run-bucket_of.c ccan/tally/test/run-divlu64.c ccan/tally/test/run-histogram.c ccan/tally/test/run-mean.c ccan/tally/test/run-median.c ccan/tally/test/run-min-max.c ccan/tally/test/run-mode.c ccan/tally/test/run-renormalize.c ccan/tally/test/run-total.c  ccan/tally/tally.o  ccan/build_assert/.ok ccan/likely/.ok 
ccan/tally/.ok: $(tally_ok_deps)
ccan/tally/.fast-ok: $(tally_ok_deps:%.ok=%.fast-ok)
