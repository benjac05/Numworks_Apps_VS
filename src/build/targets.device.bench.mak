HANDY_TARGETS += bench.flash bench.ram

bench_src = $(ion_device_bench_src) $(liba_src) $(libaxx_src) $(kandinsky_src) $(poincare_bench_src) $(omg_bench_src)
output/bench.ram.$(EXE): $(call flavored_object_for,$(bench_src),consoleuart $(MODEL) ram)
output/bench.flash.$(EXE): $(call flavored_object_for,$(bench_src),consoleuart $(MODEL) flash) $(LDDEPS)

output/bench.%.$(EXE): LDFLAGS += -Lion/src/$(PLATFORM)/epsilon-core/device/bench
output/bench.%.$(EXE): LDFLAGS += -Lion/src/$(PLATFORM)/epsilon-core/device/shared-core/flash
output/bench.flash.$(EXE): LDSCRIPT = ion/src/$(PLATFORM)/epsilon-core/device/bench/flash.ld
output/bench.ram.$(EXE): LDSCRIPT = ion/src/$(PLATFORM)/epsilon-core/device/bench/ram.ld
output/bench.%.$(EXE): LDDEPS += $(LDSCRIPT)
