iscsi_ok_deps := ccan/iscsi/test/run.c  ccan/iscsi/discovery.o ccan/iscsi/init.o ccan/iscsi/login.o ccan/iscsi/nop.o ccan/iscsi/pdu.o ccan/iscsi/scsi-command.o ccan/iscsi/scsi-lowlevel.o ccan/iscsi/socket.o  ccan/compiler/.ok 
ccan/iscsi/.ok: $(iscsi_ok_deps)
ccan/iscsi/.fast-ok: $(iscsi_ok_deps:%.ok=%.fast-ok)
