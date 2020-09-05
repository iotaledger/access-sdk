block_pool_ok_deps := ccan/block_pool/test/run.c  ccan/block_pool/block_pool.o  ccan/talloc/.ok 
ccan/block_pool/.ok: $(block_pool_ok_deps)
ccan/block_pool/.fast-ok: $(block_pool_ok_deps:%.ok=%.fast-ok)
