#
#   CGDraw - An instruction based CG renderer
#   Copyright (C) 2019 NSKernel. All rights reserved.
#
#   A lab of Computer Graphics at NJU
#
#   Makefile
#

include config

CFLAGS	= -Wall -Werror -c -I./include -MD
LDFLAGS = 

SRCS = $(shell find src -name "*.c")
OBJS = $(addprefix build/, $(addsuffix .o,  $(notdir $(basename $(SRCS)))))

LD = ld
CC = gcc

VERSIONSTR = "\"$(VERSION)\""
SUBVERSIONSTR = "\"$(SUBVERSION)\""
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG -ggdb
	BUILDSTR = "\"$(BUILD)-debug\""
else
	CFLAGS += -O2
	BUILDSTR = "\"$(BUILD)\""
endif

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LDFLAGS += 
endif

.PHONYL: clean

build/cgdraw: $(OBJS)
	@$(LD) -o $@ $(OBJS) -lc $(LDFLAGS)
	@echo "  LD    " $<

build/%.o: src/%.c
	@mkdir -p build
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)
	@echo "  CC    " $<

build/%.o: src/libbmp/%.c
	@mkdir -p build
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)
	@echo "  CC    " $<

build/%.o: src/grapheng/%.c
	@mkdir -p build
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)
	@echo "  CC    " $<

run: build/cgdraw
	./build/cgdraw

clean:
	@rm -rf build
	@echo "  REMOVED  build"

-include $(OBJS:.o=.d)