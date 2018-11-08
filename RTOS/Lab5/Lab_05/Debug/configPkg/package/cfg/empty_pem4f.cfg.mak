# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f C:\TIRTOS_Workshop_Labs\Lab_05/src/makefile.libs

clean::
	$(MAKE) -f C:\TIRTOS_Workshop_Labs\Lab_05/src/makefile.libs clean

