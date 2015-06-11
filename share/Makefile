# $Id: Makefile, zhuhk Exp $

TOP_DIR := $(shell cd .. && pwd)
include $(TOP_DIR)/share/common.mk

DEPTH = .

LIB = -L. -lmisc -lpthread -lm \
  $(NULL)

CXXFLAGS += $(CFLAGS) -Wno-deprecated 
LDFLAGS	+= $(LIB) -rdynamic -lpthread

lib_target = libmisc.a
target = misc_ut

all: $(DEP) $(target) $(lib_target)
	rm -rf output
	bash -c "mkdir -p output/{lib,include,bin}"
	cp $(target) output/bin
	cp $(lib_target) output/lib
	cp *.h output/include

clean: clean_init
	rm -f $(target) $(lib_target)

libmisc.a : misc.o testharness.o
	$(gen-static-lib)

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

# EOF
