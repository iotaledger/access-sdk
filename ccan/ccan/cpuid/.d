cpuid_ok_deps := ccan/cpuid/test/run.c  ccan/cpuid/cpuid.o  
ccan/cpuid/.ok: $(cpuid_ok_deps)
ccan/cpuid/.fast-ok: $(cpuid_ok_deps:%.ok=%.fast-ok)
