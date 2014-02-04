#
# Makefile.public
#
# $Id: Makefile zhuhk Exp $
#

DEPTH = .

MISCLIB= -L../cpp -lmisc
MISCINC= -I../cpp
INCLUDE = $(MISCINC) -I. -Wall -fPIC -DDEBUG \
	  $(NULL)

LIB	= $(MISCLIB) -llua -ldl -lpthread -lm\
	  $(NULL)

CFLAGS	= -D_GNU_SOURCE -g -pipe $(INCLUDE) \
	  $(NULL)

CXXFLAGS = $(CFLAGS) -Wno-deprecated 
LDFLAGS  = $(LIB)

target = luaexpat.so luadir.so t_lua

all: $(target) 

luaexpat.so : luaexpat.o
	g++ -shared -Wl,-soname,$@ -Wl,--whole-archive $(MISCLIB) -Wl,--no-whole-archive -o $@ $< -lexpat

luadir.so : luadir.o
	g++ -shared -Wl,-soname,$@ -Wl,--whole-archive $(MISCLIB) -Wl,--no-whole-archive -o $@ $< -lexpat

t_lua: t_lua.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean:
	rm -rf *.o *~ *.orig core.* $(target) output

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

# EOF
