output/test.js: EMSCRIPTEN_MODULARIZE = 0

HANDY_TARGETS += htmlpack
HANDY_TARGETS_EXTENSIONS += zip html

htmlpack_targets = .\

define rule_htmlpack
output/htmlpack$(1)zip: $$(addprefix output/ion/src/simulator/web/,calculator.html calculator.css) output/epsilon$(1)js ion/src/simulator/web/calculator.js ion/src/simulator/assets/background-with-shadow.webp outpution/src/simulator/assets/background.jpg outpution/src/simulator/assets/background-no-shadow.webp
endef

$(foreach target,$(htmlpack_targets),$(eval $(call rule_htmlpack,$(target))))

output/epsilon%html: output/epsilon%js output/ion/src/simulator/web/simulator.html output/release/simulator/ion/src/simulator/assets/background.jpg
	@echo "INLINE  $@"
	$(Q) ion/src/simulator/web/inline.py --image outpution/src/simulator/assets/background.jpg --script $< output/ion/src/simulator/web/simulator.html > $@
