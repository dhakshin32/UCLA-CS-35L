randlibhw.so:
	$(CC) $(CFLAGS) randlibhw.c -fPIC -shared -o randlibhw.so
randlibsw.so:
	$(CC) $(CFLAGS) randlibsw.c -fPIC -shared -o randlibsw.so
randmain:
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain
