# Define handy targets
# Those can be built easily by simply invoking "make target.ext". The named file
# will be built in output.

HANDY_TARGETS ?=
HANDY_TARGETS_EXTENSIONS ?=

# Epsilon base target

base_src = $(ion_src) $(liba_src) $(kandinsky_src) $(escher_src) $(libaxx_src) $(poincare_src) $(python_src) $(omg_src)

epsilon_src = $(base_src) $(apps_src)

output/epsilon.$(EXE): $(call flavored_object_for,$(epsilon_src),)

HANDY_TARGETS += epsilon

# Epsilon flavored targets

epsilon_flavors = \
  allow3rdparty \
  onboarding \
  onboarding.update \
  onboarding.beta \
  onboarding.beta.allow3rdparty \
  onboarding.allow3rdparty

# Clean the targets.*.mak to
define rule_for_epsilon_flavor
output/epsilon.$(1).$$(EXE): $$(call flavored_object_for,$$(epsilon_src),$(1))
endef

$(foreach flavor,$(epsilon_flavors),$(eval $(call rule_for_epsilon_flavor,$(flavor))))

HANDY_TARGETS += $(foreach flavor,$(epsilon_flavors),epsilon.$(flavor))


# Test

test_runner_src = $(base_src) $(apps_tests_src) $(runner_src) $(tests_src)

output/test.$(EXE): $(call flavored_object_for,$(test_runner_src),consoledisplay)

HANDY_TARGETS += test

# Load platform-specific targets
# We include them before the standard ones to give them precedence.
-include build/targets.$(PLATFORM).mak

# Generate handy targets rules
# Define handy targets
# Those can be built easily by simply invoking "make target.ext". The named file
# will be built in output.

HANDY_TARGETS_EXTENSIONS += $(EXE)

define handy_target_rule
.PHONY: $(1).$(2)
$(1).$(2): output/$(1).$(2)
endef

$(foreach extension,$(HANDY_TARGETS_EXTENSIONS),$(foreach executable,$(HANDY_TARGETS),$(eval $(call handy_target_rule,$(executable),$(extension)))))
