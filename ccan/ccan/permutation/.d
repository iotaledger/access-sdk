permutation_ok_deps := ccan/permutation/test/api.c  ccan/permutation/permutation.o  ccan/build_assert/.ok ccan/mem/.ok 
ccan/permutation/.ok: $(permutation_ok_deps)
ccan/permutation/.fast-ok: $(permutation_ok_deps:%.ok=%.fast-ok)
