typesafe_cb_ok_deps := ccan/typesafe_cb/test/compile_fail-cast_if_type-promotable.c ccan/typesafe_cb/test/compile_fail-typesafe_cb.c ccan/typesafe_cb/test/compile_fail-typesafe_cb_cast.c ccan/typesafe_cb/test/compile_fail-typesafe_cb_cast-multi.c ccan/typesafe_cb/test/compile_fail-typesafe_cb-int.c ccan/typesafe_cb/test/compile_fail-typesafe_cb_postargs.c ccan/typesafe_cb/test/compile_fail-typesafe_cb_preargs.c ccan/typesafe_cb/test/compile_ok-typesafe_cb_cast.c ccan/typesafe_cb/test/compile_ok-typesafe_cb-NULL.c ccan/typesafe_cb/test/compile_ok-typesafe_cb-undefined.c ccan/typesafe_cb/test/compile_ok-typesafe_cb-vars.c ccan/typesafe_cb/test/run.c   
ccan/typesafe_cb/.ok: $(typesafe_cb_ok_deps)
ccan/typesafe_cb/.fast-ok: $(typesafe_cb_ok_deps:%.ok=%.fast-ok)