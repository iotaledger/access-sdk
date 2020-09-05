ciniparser_ok_deps := ccan/ciniparser/test/run.c ccan/ciniparser/test/run-dictionary.c  ccan/ciniparser/ciniparser.o ccan/ciniparser/dictionary.o  
ccan/ciniparser/.ok: $(ciniparser_ok_deps)
ccan/ciniparser/.fast-ok: $(ciniparser_ok_deps:%.ok=%.fast-ok)
