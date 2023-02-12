PHONY: %_run
%_run: output/%.$(EXE)
	$(call rule_label,EXE)
	$(Q) ./$^
