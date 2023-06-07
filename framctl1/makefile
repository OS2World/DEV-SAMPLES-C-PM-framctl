# nmake makefile
#
# Tools used:
#  Compile::Watcom Resource Compiler
#  Compile::GNU C
#  Make: nmake or GNU make
all : framectl.exe framectl.hlp 

framectl.exe : framectl.obj framectl.res framectl.def
	gcc -Zomf framectl.obj framectl.def -o framectl.exe
	wrc framectl.res


framectl.obj : framectl.c framectl.h
	gcc -Wall -Zomf -c -O2 framectl.c -o framectl.obj

framectl.res : framectl.rc 
	wrc -r framectl.rc

framectl.hlp: framectl.ipf
	wipfc -l en_US framectl.ipf

clean :
	rm -rf *exe *res *obj *hlp