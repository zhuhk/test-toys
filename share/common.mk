# $Id: Makefile, zhuhk Exp $

init:all

CFLAGS = -g -I. -Wall -fPIC -D_GNU_SOURCE 
CXXFLAGS = -g
LDFLAGS = -rdynamic

CC = gcc
MAKE = make
CXX = g++
AR = ar

define gen-static-lib
  $(AR) cr $@ $^
  ranlib $@
endef

.PHONY: clean all init

clean_init:
	@find . -name "*.o" |xargs rm -f
	@find . -name "*.d" |xargs rm -f
	@find . -name "tags" |xargs rm -f
	@find . -name "*~" |xargs rm -f
	@find . -name ".*.swp" |xargs rm -f
	@find . -name "core*" |xargs rm -f
	@find . -name "output" |xargs rm -rf

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

%.o : %.c
	$(CC) -c $< -o $@ $(CXXFLAGS) 
%.o : %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS) 
%.o : %.cpp 
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

define gen-deps
  @set -e; rm -f $@; \
  $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
  sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ >$@; \
  rm -f $@.$$$$
endef

SRC = $(wildcard *.c *.cpp *.cc)
DEP := $(SRC:.c=.d)
DEP := $(DEP:.cpp=.d)
DEP := $(DEP:.cc=.d)
-include $(DEP)

%.d: %.cpp
	$(gen-deps)
%.d: %.cc
	$(gen-deps)
%.d: %.c
	$(gen-deps)
