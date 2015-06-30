#
# $Id: Makefile, zhuhk Exp $
#

TOP_DIR := $(shell cd .. && pwd)
include $(TOP_DIR)/share/common.mk

LIB = -L$(SHARE_DIR) -lmisc -lpthread -lm \
  -L$(THIRD_DIR)/leveldb/lib -lleveldb -lsnappy\
  -L$(THIRD_DIR)/jsoncpp/lib -ljsoncpp\
  $(NULL)

CFLAGS = -I$(THIRD_DIR)/leveldb/include \
		 -I$(THIRD_DIR)/jsoncpp/include \
		  $(NULL)
CXXFLAGS = $(CFLAGS)
LDFLAGS := $(LIB) -rdynamic -lpthread
target = t t1 t2 t3 t4 t_ev spawn dynlib_main dynlib.so dynlib1.so

external:
	make -C $(SHARE_DIR)

all: external $(target) 
	mkdir -p output
	cp $(target) output
	cp *.h output

spawn : spawn.o
	$(CXX) -o $@ $< $(LDFLAGS) 

## -fpic is equal to -fPIC
dynlib.so : dynlib.cpp dynlib.h
	$(CXX) -o $@ $< -fpic -shared
dynlib1.so : dynlib1.cpp
	$(CXX) -o $@ $< -fPIC -shared

dynlib_main : dynlib_main.o
	$(CXX) -o $@ $< $(LDFLAGS) -ldl

t_ev: t_ev.o
	$(CXX) -o $@ $< $(LDFLAGS) -levent
t: t.o
	$(CXX) -o $@ $< $(LDFLAGS) 

t3: t3.cpp
	$(CXX) -o $@ $<  -std=c++11 -lpthread

t4: t4.cpp
	$(CXX) -o $@ $<  -std=c++11 -lpthread

t1: t1.cpp
	$(CXX) -o $@ $< -fopenmp $(LDFLAGS) 

t_json: t_json.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

t_lvdb: t_lvdb.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

t2: t2.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean: clean_init
	rm -rf *.o *~ *.orig core.* $(target) output

#-include $(DEP)
# EOF
