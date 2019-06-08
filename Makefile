#
#   CGDraw - An instruction based CG renderer
#   Copyright (C) 2019 NSKernel. All rights reserved.
#
#   A lab of Computer Graphics at NJU
#
#   Makefile
#

include config

CFLAGS	= -Wall -Werror -Wno-unused-result -Wno-unused-value -c -I./include -MD
LDFLAGS = -lm

SRCS = $(shell find src -name "*.c")
OBJS = $(addprefix build/, $(addsuffix .o,  $(notdir $(basename $(SRCS)))))

LD = ld
CC = gcc

VERSIONSTR = "\"$(VERSION)\""
SUBVERSIONSTR = "\"$(SUBVERSION)\""
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG -ggdb
	LDFLAGS += -DDEBUG -ggdb
	BUILDSTR = "\"$(BUILD)-debug\""
else
	CFLAGS += -O2
	LDFLAGS += -O2
	BUILDSTR = "\"$(BUILD)\""
endif

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LDFLAGS += 
endif

.PHONYL: clean

build/cgdraw: $(OBJS)
	@echo "  LD    " $@
	@$(CC) -o $@ $(OBJS) -lc $(LDFLAGS)
	@echo $@ "is ready"

build/%.o: src/%.c
	@mkdir -p build
	@echo "  CC    " $@
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)

build/%.o: src/libbmp/%.c
	@mkdir -p build
	@echo "  CC [L]" $@
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)

build/%.o: src/grapheng/%.c
	@mkdir -p build
	@echo "  CC    " $@
	@$(CC) $(CFLAGS) -o $@ $< -D VERSION=$(VERSIONSTR) -D SUBVERSION=$(SUBVERSIONSTR) -D BUILD=$(BUILDSTR)

run: build/cgdraw
	./build/cgdraw

clean:
	@rm -rf build
	@echo "  REMOVED  build"

-include $(OBJS:.o=.d)
