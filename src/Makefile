CC=gcc
CFLAG=-O -o

# static libs
CSTLFLAG= -O -c -o	
# shared libs
CSHLFLAG= -O -shared -fPIC -o

LD=ld

LIBDIR= ./ 

all:clean svma svm

linklist.o:linklist.h.gch
	$(CC) $(CSTLFLAG) linklist.o linklist.c
linklist.h.gch:
	$(CC) linklist.h
vmtypes.o:vmtypes.h.gch linklist.o
	$(CC) $(CSTLFLAG) vmtypes.o vmtypes.c
vmtypes.h.gch:
	$(CC) vmtypes.h
vconsts.o:vconsts.h.gch
	$(CC) $(CSTLFLAG) vconsts.o vconsts.c
vconsts.h.gch:
	$(CC) vconsts.h
vmemory.o: vconsts.o vmtypes.o vmemory.h.gch
	$(CC) $(CSTLFLAG) vmemory.o vmemory.c
vmemory.h.gch:
	$(CC) vmemory.h
vmachine.o:vmachine.h.gch vconsts.o vmtypes.o vmemory.o
	$(CC) $(CSTLFLAG) vmachine.o vmachine.c
vmachine.h.gch:
	$(CC) vmachine.h
# svm seems to bee an executable (contains main)
svm.o:vmachine.o vconsts.o vmemory.o vmtypes.o
	$(CC) $(CSTLFLAG) svm.o svm.c
svm:svm.o
	$(CC) svm.o vmachine.o vmemory.o vconsts.o linklist.o vmtypes.o -o svm
svma.o:vmachine.o vmtypes.o vconsts.o vmemory.o
	$(CC) $(CSTLFLAG) svma.o svma.c
svma: svma.o
	$(CC) svma.o vmachine.o vmemory.o vconsts.o linklist.o vmtypes.o -o svma
clean:
	rm -f *.o || true;\
	rm -f *.gch || true;\
	rm -f *.a || true;\
	rm -f *.so || true

