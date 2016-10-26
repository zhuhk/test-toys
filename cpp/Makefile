#
# $Id: Makefile, zhuhk Exp $
#

TOP_DIR := ..
include $(TOP_DIR)/share/common.mk

LIB = -L$(SHARE_DIR) -lmisc -lm \
  -L$(THIRD_DIR)/leveldb/lib -lleveldb \
  -L$(THIRD_DIR)/jsoncpp/lib -ljsoncpp\
  $(NULL)

CFLAGS = -std=c++11 -g -O3 -fPIC -I$(THIRD_DIR)/leveldb/include -I../share\
		 -I$(THIRD_DIR)/jsoncpp/include -I$(THIRD_DIR)\
		 -I$(THIRD_DIR)/sparsehash/include -I$(THIRD_DIR)\
		  $(NULL)
CXXFLAGS = $(CFLAGS)
CPPFLAGS = $(CFLAGS)
LDFLAGS := -rdynamic $(LIB) 
target = t t1 t2 t_c11 t4 t_ev spawn dynlib_main dynlib.so dynlib1.so.1 t_rapidjson

external:
	make -C $(SHARE_DIR)

all: external $(target) 
	mkdir -p output
	cp $(target) output
	cp *.h output
	ln -sf dynlib1.so.1 dynlib_1.so

spawn : spawn.o
	$(CXX) -o $@ $< $(LDFLAGS) 

## -fpic is equal to -fPIC
dynlib.so : dynlib.o dynlib.h
	#$(CXX) -o $@ $< -Wl,-soname,$@ -fpic -shared 
	$(CXX) $< -o $@ -fpic -shared 
dynlib2.so : dynlib2.o dynlib.h
	#$(CXX) -o $@ $< -Wl,-soname,$@ -fpic -shared
	$(CXX) $< -o $@ -Wl,-soname,$@ -fpic -shared
dynlib1.so.1 : dynlib1.o
	#$(CXX) -o $@ $< -Wl,-soname,$@ -fpic -shared
	$(CXX) $< -o $@ -fpic -shared
dynlib1.so.2 : dynlib1_2.o
	$(CXX) -o $@ $< -Wl,-soname,$@ -fpic -shared

dynlib_main : dynlib_main.o
	$(CXX) -o $@ $< $(LDFLAGS) -ldl #-ltcmalloc

t_ev: t_ev.o
	$(CXX) -o $@ $< $(LDFLAGS) -levent

t: t.o
	$(CXX) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

t_c11: t_c11.o
	$(CXX) -o $@ $<  -std=c++11 -I$(SHARE_DIR) -L$(SHARE_DIR) -lmisc -pthread

t4: t4.o
	$(CXX) -o $@ $<  -std=c++11 -lpthread

t1: t1.o
	$(CXX) -o $@ $< -std=c++11 -fopenmp $(LDFLAGS) 

t_rapidjson: t_rapidjson.o
	$(CXX) -o $@ $< -std=c++11 -fopenmp $(LDFLAGS) 

t_json: t_json.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

t_lvdb: t_lvdb.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

t2: t2.o 
	$(CXX) -o $@ $< $(LDFLAGS) 

misc_ut: misc_ut.o libmisc.a 
	$(CXX) -o $@ $< $(LDFLAGS) 

clean: clean_init
	rm -rf *.so *.o *~ *.orig core.* $(target) output

#-include $(DEP)
# EOF
