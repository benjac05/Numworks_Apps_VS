ifeq ($(APPLE_PLATFORM),ios-simulator)
.PHONY: %_run
%_run: output/%.app
	$(call rule_label,XCRUN)
	$(Q) xcrun simctl install booted $^
endif
