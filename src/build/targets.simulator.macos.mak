.PHONY: %_run
%_run: output/%.app
	$(call rule_label,OPEN)
	$(Q) open $^

# Reload the simulator
reload: default
	$(Q) pgrep Epsilon && pkill -USR1 Epsilon || echo "No Epsilon executable running"
	$(Q) cmd="open output/$(firstword $(HANDY_TARGETS)).$(firstword $(HANDY_TARGETS_EXTENSIONS)) --args --load-state-file /tmp/numworks.reload.nws";\
	eval $$cmd || $$cmd
