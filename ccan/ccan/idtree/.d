idtree_ok_deps := ccan/idtree/test/run.c ccan/idtree/test/run-wrap.c  ccan/idtree/idtree.o  ccan/tal/.ok 
ccan/idtree/.ok: $(idtree_ok_deps)
ccan/idtree/.fast-ok: $(idtree_ok_deps:%.ok=%.fast-ok)
