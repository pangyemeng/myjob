# root directory
ROOT_DIR = $(shell pwd)

# sub directory
SUBDIRS = 

#head
UTIL_HEAD = $(ROOT_DIR)/../util/include
HIREDIS_HEAD = $(ROOT_DIR)/../hiredis
#lib

#some display variables
NORMAL="\\033[0;39m"
SUCCESS="\\033[1;32m"
FAILURE="\\033[1;31m"

#targets
LIBTARGET = libmyjob_parser.so

#objects
OBJS = public.o rule.o html_util.o job_parser.o 

#special CFLAGS LDFLAG
CFLAGS += -I ./include -I$(UTIL_HEAD) -I$(HIREDIS_HEAD)
CFLAGS += -fPIC
LDFLAGS += -lhiredis -lzdb

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
