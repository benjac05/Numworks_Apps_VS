.PHONY: %_run
%_run: output/%.$(EXE)
	$(call rule_label,EXE)
	$(Q) ./$^

# Reload the simulator
reload: default
	$(Q) pgrep Epsilon && pkill -USR1 Epsilon || echo "No Epsilon executable running"
	$(Q) ./output/$(firstword $(HANDY_TARGETS)).$(firstword $(HANDY_TARGETS_EXTENSIONS)) --load-state-file /tmp/numworks.reload.nws

# If MICROPY_NLR_SETJMP is 0, the MicroPython NLR done by
# python/src/py/nlrx64.c crashes on linux.
SFLAGS += -DMICROPY_NLR_SETJMP=1
