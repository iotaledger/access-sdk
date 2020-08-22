darray_ok_deps := ccan/darray/test/lotsOfNumbers.h ccan/darray/test/lotsOfStrings.h ccan/darray/test/run.c ccan/darray/test/testLits.h   
ccan/darray/.ok: $(darray_ok_deps)
ccan/darray/.fast-ok: $(darray_ok_deps:%.ok=%.fast-ok)
