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
  -Lthird/jsoncpp/lib -ljsoncpp\
  -Lthird/leveldb/lib -lleveldb\
  $(NULL)

CFLAGS = -D_GNU_SOURCE -g -pipe $(INCLUDE) \
		  $(NULL)

CXXFLAGS = $(CFLAGS) -Wno-deprecated 
LDFLAGS	= $(LIB) -rdynamic

target = t libmisc.a misc_ut spawn t_json t_lvdb

all: $(target) $(lib_target)
	mkdir -p output
	cp $(target) output
	cp *.h output

libmisc.a : misc.o testharness.o
	ar cr $@ $^
	ranlib $@

spawn : spawn.o
	$(CXX) -o $@ $< $(LDFLAGS) 

t: t.o libmisc.a
	$(CXX) -o $@ $< $(LDFLAGS) 

t_json: t_json.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

t_lvdb: t_lvdb.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean:
	rm -rf *.o *~ *.orig core.* $(target) output

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

# EOF
