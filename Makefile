# root directory
ROOT_DIR = $(shell pwd)

#some display variables
NORMAL="\\033[0;39m"
SUCCESS="\\033[1;32m"
FAILURE="\\033[1;31m"

# sub directory
SUBDIRS = hiredis util common db parser task

# myjob library
MYJOB_LIB = $(ROOT_DIR)/common/libmyjob_common.so $(ROOT_DIR)/parser/libmyjob_parser.so $(ROOT_DIR)/task/libtask_create.so $(ROOT_DIR)/db/libmyjob_db.so $(ROOT_DIR)/util/libmyjob_util.so $(ROOT_DIR)/hiredis/libhiredis.so 

# src file
SRC = main.c 

# compilation
INCLUDE_DIR = -I$(ROOT_DIR)/parser/include -I$(ROOT_DIR)/task/include -I$(ROOT_DIR)/hiredis -I$(ROOT_DIR)/common/include -I$(ROOT_DIR)/db/include -I$(ROOT_DIR)/util/include -I$(ROOT_DIR)/opensource/zdb
LDFLAGS = -lpthread -ljansson -lzdb -lpcre -lhiredis 
CFLAGS = -rdynamic $(INCLUDE_DIR) -Wall -fPIC

# optmimization
ifdef O3
CFLAGS += -O3
 ifndef CHECKOFF
 CHECKOFF = 1
 endif
else
#CFLAGS += -g -ggdb -dr
CFLAGS += -g -ggdb -O0
endif

# main cflags
MCFLAGS = $(CFLAGS)

#compiler variable
CC              = $(CROSS_COMPILE)gcc
CXX             = $(CROSS_COMPILE)g++
AR              = $(CROSS_COMPILE)ar
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
RANLIB  = $(CROSS_COMPILE)ranlib
NM              = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# To make it visible
export CXX CXXPP ROOT_DIR CFLAGS LDFLAGS INCLUDE_DIR MYJOB_LIB

all: subdir myjob

help:
	@echo " "

# version name
ifndef VER
VER = $(shell date +%Y_%m_%d)
endif

subdir:
	@for dir in $(SUBDIRS) ; \
           do $(MAKE) -C $$dir || exit 1; \
         done

myjob: $(SRC:.c=.o) $(MYJOB_LIB)
	$(CXX) $(MCFLAGS) -o $@ $(SRC:.c=.o) -L$(MYJOB_LIB) $(LDFLAGS)
	@echo -en $(SUCCESS);
	@echo "make myjob success"
	@echo -en $(NORMAL);
	
clean: 
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir clean; done
	rm -f myjob *.o *~ log.* .depend val.* *.expand 
	@echo -en $(SUCCESS);
	@echo "make clean successfully"
	@echo -en $(NORMAL);
	
%.o: %.c
	$(CXX) $(MCFLAGS) -c -o $@ $< 


.depend: $(SRC)
	$(CXX) -M $(MCFLAGS) $(SRC) > $@


sinclude .depend
