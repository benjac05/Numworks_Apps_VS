.PHONY: %_run
%_run: output/%.apk
	$(call rule_label,ADB)
	$(Q) adb install $<
