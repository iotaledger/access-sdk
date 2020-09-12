edit_distance_ok_deps := ccan/edit_distance/test/api.c ccan/edit_distance/test/run-types-short.c ccan/edit_distance/test/run-types-struct.c ccan/edit_distance/test/run-types-wchar.c ccan/edit_distance/test/run-weights.c  ccan/edit_distance/edit_distance.o ccan/edit_distance/edit_distance_dl.o ccan/edit_distance/edit_distance_lcs.o ccan/edit_distance/edit_distance_lev.o ccan/edit_distance/edit_distance_rdl.o  
ccan/edit_distance/.ok: $(edit_distance_ok_deps)
ccan/edit_distance/.fast-ok: $(edit_distance_ok_deps:%.ok=%.fast-ok)
