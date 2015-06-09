#
# Makefile
#
# $Id: Makefile, zhuhk Exp $
#

DEPTH = .

INCLUDE = -I.. -I. -Wall -fPIC \
  -Ithird/leveldb/include\
  $(NULL)

LIB = -L. -lmisc -lpthread -lm \
  $(NULL)

CFLAGS = -D_GNU_SOURCE -g -pipe $(INCLUDE) \
		  $(NULL)

CXXFLAGS = $(CFLAGS) -Wno-deprecated 
LDFLAGS	= $(LIB) -rdynamic -lpthread

lib_target = libmisc.a
target = misc_ut

all: $(target) $(lib_target)
	rm -rf output
	bash -c "mkdir -p output/{lib,include,bin}"
	mv $(target) output/bin
	mv $(lib_target) output/lib
	cp *.h output/include

libmisc.a : misc.o testharness.o
	ar cr $@ $^
	ranlib $@

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean:
	rm -rf *.o *~ *.orig core.* output

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

# EOF
