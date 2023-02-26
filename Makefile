Q ?= @
#CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
#RANLIB = arm-none-eabi-ranlib
#STRIP = arm-none-eabi-strip
NWLINK = npx --yes -- nwlink@0.0.16
LINK_GC = 1
LTO = 1

#KANDINSKY OBJECT FILES

objs = $(addprefix output/kandinsky/fonts/,\
  SmallFont.o \
  LargeFont.o \
)

objs += $(addprefix output/kandinsky/src/,\
  color.o \
  context_circle.o \
  context_line.o \
  context_pixel.o \
  context_rect.o \
  context_text.o \
  font.o \
  framebuffer.o \
  ion_context.o \
  point.o \
)

#ION OBJECT FILES
objs += $(addprefix output/ion/src/device/userland/drivers/,\
  authentication.o \
  backlight.o \
  battery.o \
  board.o \
  display.o \
  events.o \
  keyboard.o \
  led.o \
  power.o \
  random.o \
  timing.o \
)


#REMOVED line.o
#REMOVED rect.o



#MAIN OBJECT FILES

objs += $(addprefix output/,\
  icon.o \
  main.o \
)

CPPFLAGS = -std=c++11 -fno-exceptions
CPPFLAGS += $(shell $(NWLINK) eadk-cflags)
CPPFLAGS += -Os -Wall
CPPFLAGS += -ggdb
CPPFLAGS += -Isrc/kandinsky
CPPFLAGS += -Isrc/ion
LDFLAGS = -Wl,--relocatable
LDFLAGS += -nostartfiles
LDFLAGS += --specs=nosys.specs

# LDFLAGS += --specs=nosys.specs # Alternatively, use full-fledged newlib

#include src/kandinsky/Makefile


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

.PHONY: build
build: output/app.nwa

.PHONY: check
check: output/app.bin

.PHONY: run
run: output/app.nwa
	@echo "INSTALL $<"
	$(Q) $(NWLINK) install-nwa $<

output/%.bin: output/%.nwa
	@echo "BIN     $@"
	$(Q) $(NWLINK) nwa-bin $< $@

output/%.elf: output/%.nwa
	@echo "ELF     $@"
	$(Q) $(NWLINK) nwa-elf $< $@

output/app.nwa: $(objs)
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