epsilon_target_variants = test epsilon $(addprefix epsilon.,$(epsilon_flavors))
dfu_targets = $(addprefix output/,$(addsuffix .dfu,$(epsilon_target_variants)))
.PHONY: $(dfu_targets)
$(dfu_targets): TARGET_STEM = $(subst .epsilon,,$(patsubst output/%.dfu,.%,$@))

define target_variants_for_component
$(subst .epsilon,,$(addprefix $(1).,$(epsilon_target_variants)))
endef

define flavored_dependencies_for_target
$(1): $$(call flavored_object_for,$$($(addsuffix $(if $(findstring test,$(1)),_test_src,_src),$(2))),$(3) $(MODEL) $(THIRD_PARTY_FLAVOR) $(patsubst output/$(2)%.$(EXE),%,$(1)))
endef

include build/targets.device.$(MODEL).mak
-include build/targets.device.$(MODEL).$(FIRMWARE_COMPONENT).mak

HANDY_TARGETS_EXTENSIONS += dfu hex bin

$(eval $(call rule_for, \
  RAMSIZE, %_ram_map.png, %.elf, \
  $$(PYTHON) build/device/ram_map.py $$< $$@, \
  local \
))

output/immv.html: build/device/immv/$(MODEL).json output/kernel.A.sections.json output/kernel.A.symbols.json output/userland.A.sections.json output/userland.A.symbols.json
	@echo "INLINE  $@"
	$(Q) ion/src/simulator/web/inline.py --script build/device/immv/immv.js build/device/immv/immv.html > $@
	@echo "INJMAP  $@"
	$(Q) echo "<script>" >> $@
	$(Q) for MAP in $^;do echo "addMaps(" >> $@; cat $${MAP} >> $@;echo ")" >> $@;done
	$(Q) echo "</script>" >> $@

.PHONY: %_size
%_size: output/%.$(EXE)
	@echo "========= BUILD OUTPUT ========"
	@echo "File:  $<"
	@$(SIZE) $< | tail -n 1 | awk '{print "Code:  " $$1 " bytes";print "Data:  " $$2 " bytes"; print "Total: " int(($$1+$$2)/1024) " kB (" $$1 + $$2 " bytes)";}'
	@echo "==============================="

.PHONY: %_run
%_run: output/%.$(EXE)
	$(GDB) -x build/$(PLATFORM)/gdb_script.gdb $<

output/%.sections.json: output/%.elf
	@echo "ELF2JS  $@"
	$(Q) $(PYTHON) build/device/immv/elf2json.py -S $^ > $@

output/%.symbols.json: output/%.elf
	@echo "ELF2JS  $@"
	$(Q) $(PYTHON) build/device/immv/elf2json.py -s 20 $^ > $@

output/%.map: output/%.elf
	@echo "LDMAP   $@"
	$(Q) $(LD) $^ $(LDFLAGS) -Wl,-M -Wl,-Map=$@ -o /dev/null

.PHONY: %_memory_map
%_memory_map: output/%.map
	@echo "========== MEMORY MAP ========="
	$(Q) awk -f build/device/memory_map.awk < $<
	@echo "==============================="

.PHONY: openocd
openocd:
	openocd -f build/$(PLATFORM)/openocd.$(MODEL).cfg

SFLAGS += -DEMBED_EXTRA_DATA=$(EMBED_EXTRA_DATA)

output/%.$(EXE): LDFLAGS += -Loutput/ion/src/$(PLATFORM)/shared/flash/$(MODEL)
output/%.$(EXE): LDDEPS += output/ion/src/$(PLATFORM)/shared/flash/$(MODEL)/board.ld

output/ion/src/$(PLATFORM)/shared/flash/$(MODEL)/board.ld: ion/src/$(PLATFORM)/include/$(MODEL)/config/board.h | $$(@D)/.
	$(Q) $(CXX) $(SFLAGS) -E $< -o $(@:.ld=.h)
	$(Q) awk '/^constexpr/ {$$1=$$2=""; sub(";.*", ";"); print}; /^static_assert/ {sub("static_assert", "ASSERT"); print}' $(@:.ld=.h) >$@

# Generate objects files containing the raw bootloader binary, to be linked
# into the kernel.
# FIXME We would like this target to depend on bootloader.elf, built in the
# bootloader folder, but adding this prerequisite would cause MAKE to attempt
# to rebuild bootloader.elf with the kernel configuration. As such, we force
# this rule to always run, knowing that the recursive MAKE will perform the
# right amount of work.
output/trampoline.o: output/bootloader.o # This is a workaround because grouped target (&:) is part of Make version > 4.2
output/bootloader.o: force_remake
	$(MAKE) FIRMWARE_COMPONENT=bootloader bootloader.elf
	$(Q) $(OBJCOPY) -O binary -S -R .trampoline -R .pseudo_otp -R .unused_flash $(subst $(FIRMWARE_COMPONENT),bootloader,output)/bootloader.elf output/bootloader.bin
	$(Q) $(OBJCOPY) -O binary -S -j .trampoline $(subst $(FIRMWARE_COMPONENT),bootloader,output)/bootloader.elf output/trampoline.bin
	$(Q) $(OBJCOPY) -I binary -O elf32-littlearm -B arm --rename-section .data=.bootloader output/bootloader.bin output/bootloader.o
	$(Q) $(OBJCOPY) -I binary -O elf32-littlearm -B arm --rename-section .data=.trampoline output/trampoline.bin output/trampoline.o
