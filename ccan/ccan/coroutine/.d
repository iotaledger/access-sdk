coroutine_ok_deps := ccan/coroutine/test/api-1.c ccan/coroutine/test/api-2.c ccan/coroutine/test/api-3.c  ccan/coroutine/coroutine.o  ccan/build_assert/.ok ccan/compiler/.ok ccan/ptrint/.ok ccan/typesafe_cb/.ok 
ccan/coroutine/.ok: $(coroutine_ok_deps)
ccan/coroutine/.fast-ok: $(coroutine_ok_deps:%.ok=%.fast-ok)
