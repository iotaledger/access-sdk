ccan_tokenizer_ok_deps := ccan/ccan_tokenizer/test/run.c ccan/ccan_tokenizer/test/run-simple-token.c  ccan/ccan_tokenizer/ccan_tokenizer.o ccan/ccan_tokenizer/charflag.o ccan/ccan_tokenizer/dict.o ccan/ccan_tokenizer/queue.o ccan/ccan_tokenizer/read_cnumber.o ccan/ccan_tokenizer/read_cstring.o  ccan/darray/.ok ccan/talloc/.ok 
ccan/ccan_tokenizer/.ok: $(ccan_tokenizer_ok_deps)
ccan/ccan_tokenizer/.fast-ok: $(ccan_tokenizer_ok_deps:%.ok=%.fast-ok)
