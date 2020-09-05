a_star_ok_deps := ccan/a_star/test/run.c  ccan/a_star/a_star.o  
ccan/a_star/.ok: $(a_star_ok_deps)
ccan/a_star/.fast-ok: $(a_star_ok_deps:%.ok=%.fast-ok)
