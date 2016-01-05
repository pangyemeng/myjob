# root directory
ROOT_DIR = $(shell pwd)

# sub directory
SUBDIRS = 

#head
UTIL_HEAD = $(ROOT_DIR)/../util/include
ZDB_HEAD = $(ROOT_DIR)/../opensource/zdb
DB_HEAD = $(ROOT_DIR)/../db/include
COMMON_HEAD = $(ROOT_DIR)/../common/include
#lib

#some display variables
NORMAL="\\033[0;39m"
SUCCESS="\\033[1;32m"
FAILURE="\\033[1;31m"

#targets
LIBTARGET = libtask_create.so

#objects
OBJS = task_create.o

#special CFLAGS LDFLAG
CFLAGS += -I ./include -I$(UTIL_HEAD) -I$(ZDB_HEAD) -I$(DB_HEAD) -I$(COMMON_HEAD)
CFLAGS += -fPIC
LDFLAGS += -lpthread -lzdb

.c.o:
	$(CXX) $(CFLAGS) -o $@ -c $<

.PHONY:all
all:lib

lib:$(LIBTARGET)

$(LIBTARGET):$(OBJS)
	$(CXX) $(CFLAGS) -fPIC -shared -o $(LIBTARGET) $(OBJS) $(LDFLAGS)
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
