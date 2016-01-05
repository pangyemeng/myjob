# root directory
ROOT_DIR = $(shell pwd)

# sub directory
SUBDIRS = 

#head
HIREDIS_HEAD = $(ROOT_DIR)/../hiredis
ZDB_HEAD = $(ROOT_DIR)/../opensource/zdb
COMMON_HEAD = $(ROOT_DIR)/../common/include
UTIL_HEAD = $(ROOT_DIR)/../util/include

#lib
HIREDIS_LIB = $(ROOT_DIR)/../hiredis

#some display variables
NORMAL="\\033[0;39m"
SUCCESS="\\033[1;32m"
FAILURE="\\033[1;31m"

#targets
LIBTARGET = libmyjob_db.so

#objects
OBJS = redis_db.o sql_db.o db.o

#special CFLAGS LDFLAG
CFLAGS += -I$(HIREDIS_HEAD) -I$(ZDB_HEAD) -I$(COMMON_HEAD) -I$(UTIL_HEAD)
CFLAGS += -I ./include
CFLAGS += -fPIC 

.c.o:
	$(CXX) $(CFLAGS) -o $@ -c $<

.PHONY:all
all:lib

lib:$(LIBTARGET)

$(LIBTARGET):$(OBJS)
	$(CXX) $(CFLAGS) -shared -o $(LIBTARGET) $(OBJS) $(LDFLAGS)
	@echo -en $(SUCCESS);
	@echo "make lib success"
	@echo -en $(NORMAL);

install: $(LIBTARGET)
	@mkdir -p $(FIRM_LIB_DIR)
	@cd $(TOPDIR) && $(CP) $(LIBTARGET) $(FIRM_LIB_DIR)
	@echo -en $(SUCCESS);
	@echo "install successfully";
	@echo -en $(NORMAL);

clean:
	@rm -fr $(LIBTARGET) $(OBJS)
	@echo -en $(SUCCESS);
	@echo "make clean successfully"
	@echo -en $(NORMAL);
