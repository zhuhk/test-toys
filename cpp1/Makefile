#
# Makefile
#
# $Id: Makefile, zhuhk Exp $
#

DEPTH = .

INCLUDE = -I.. -I. -Wall -fPIC \
	  $(NULL)

LIB	= -L. -lmisc -lpthread -lm \
	  $(NULL)

CFLAGS	= -D_GNU_SOURCE -g -pipe $(INCLUDE) \
		  $(NULL)

CXXFLAGS	= $(CFLAGS) -Wno-deprecated 
LDFLAGS		= $(LIB)

target = t1 t libmisc.a misc_ut

all: $(target) $(lib_target)
	#mkdir -p output
	#mv $(target) output
	#cp *.h output

libmisc.a : misc.o testharness.o
	ar cr $@ $^
	ranlib $@

t1: t1.o
	$(CXX) -o $@ $<

t: t.o libmisc.a
	$(CXX) -o $@ $< $(LDFLAGS) 

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean:
	rm -rf *.o *~ *.orig core.* core $(target) output

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

# EOF
