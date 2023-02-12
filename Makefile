Q ?= @
#CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
#RANLIB = arm-none-eabi-ranlib
#STRIP = arm-none-eabi-strip
NWLINK = npx --yes -- nwlink@0.0.16
LINK_GC = 1
LTO = 1


objs = $(addprefix output/kandinsky/src/,\
  color.o \
  font.o \
  context.o \
  coordinate.o \
  dot.o \
  framebuffer.o \
  ion_context.o \
  measuring_context.o \
  palette.o \
  pixel_cache.o \
  point.o \
  rect.o \
  size.o \
)


objs += $(addprefix output/,\
  eadkpp.o \
  icon.o \
  main.o \
)

CPPFLAGS = -std=c++11 -fno-exceptions
CPPFLAGS += $(shell $(NWLINK) eadk-cflags)
CPPFLAGS += -Os -Wall
CPPFLAGS += -ggdb
CPPFLAGS += -Isrc/kandinsky
LDFLAGS = -Wl,--relocatable
LDFLAGS += -nostartfiles
LDFLAGS += --specs=nosys.specs

# LDFLAGS += --specs=nosys.specs # Alternatively, use full-fledged newlib

ifeq ($(LINK_GC),1)
CPPFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,-e,main -Wl,-u,eadk_app_name -Wl,-u,eadk_app_icon -Wl,-u,eadk_api_level
LDFLAGS += -Wl,--gc-sections
endif

ifeq ($(LTO),1)
CPPFLAGS += -flto -fno-fat-lto-objects
CPPFLAGS += -fwhole-program
CPPFLAGS += -fvisibility=internal
LDFLAGS += -flinker-output=nolto-rel
endif

include src/kandinsky/Makefile

.PHONY: build
build: output/kandinsky.nwa

.PHONY: check
check: output/kandinsky.bin

.PHONY: run
run: output/kandinsky.nwa
	@echo "INSTALL $<"
	$(Q) $(NWLINK) install-nwa $<

output/%.bin: output/%.nwa
	@echo "BIN     $@"
	$(Q) $(NWLINK) nwa-bin $< $@

output/%.elf: output/%.nwa
	@echo "ELF     $@"
	$(Q) $(NWLINK) nwa-elf $< $@

output/kandinsky.nwa: $(objs)
	@echo "LD      $@"
	$(Q) $(CXX) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

output/%.o: src/%.cpp
	@mkdir -p $(@D)
	@echo "CXX      $^"
	$(Q) $(CXX) $(CPPFLAGS) -c $^ -o $@


output/icon.o: src/icon.png
	@echo "ICON    $<"
	$(Q) $(NWLINK) png-icon-o $< $@

.PHONY: clean
clean:
	@echo "CLEAN"
	$(Q) rm -rf output