deque_ok_deps := ccan/deque/test/api1.c ccan/deque/test/run2.c ccan/deque/test/run3.c  ccan/deque/deque.o  
ccan/deque/.ok: $(deque_ok_deps)
ccan/deque/.fast-ok: $(deque_ok_deps:%.ok=%.fast-ok)
