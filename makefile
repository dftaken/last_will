# root makefile

include make.vars

all:
	$(MAKE) -C src MAKE_VARS=$(CURDIR)/make.vars all

clean:
	rm $(BLD_DIR)/*