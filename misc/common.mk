# $Id: Makefile, zhuhk Exp $

init:all

CFLAGS = -g
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

clean:
	find . -name "*.o" |xargs rm -f
	find . -name "tags" |xargs rm -f
	find . -name "*~" |xargs rm -f
	find . -name ".*.swp" |xargs rm -f
	find . -name "core*" |xargs rm -f
	find . -name "output" |xargs rm -rf

tags : $(wildcard *.cpp *.c *.h *.cc)
	ctags $^

%.o : %.c
	$(CC) -c $< -o $@ $(CXXFLAGS) 
%.o : %.cpp %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS) 
