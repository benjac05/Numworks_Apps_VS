HANDY_TARGETS += flasher

flasher_src = $(ion_device_flasher_src) $(liba_src) $(liba_flasher_src) $(kandinsky_src)
output/flasher.$(EXE): $(call flavored_object_for,$(flasher_src),$(MODEL))
output/flasher.$(EXE): LDFLAGS += -Lion/src/$(PLATFORM)/epsilon-core/device/flasher
output/flasher.$(EXE): LDFLAGS += -Lion/src/$(PLATFORM)/epsilon-core/device/shared-core/flash
output/flasher.$(EXE): LDSCRIPT = ion/src/$(PLATFORM)/epsilon-core/device/flasher/ram.ld
output/flasher.$(EXE): LDDEPS += $(LDSCRIPT)
