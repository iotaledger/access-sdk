ttxml_ok_deps := ccan/ttxml/test/run.c  ccan/ttxml/ttxml.o  
ccan/ttxml/.ok: $(ttxml_ok_deps)
ccan/ttxml/.fast-ok: $(ttxml_ok_deps:%.ok=%.fast-ok)
