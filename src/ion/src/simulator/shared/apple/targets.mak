.PRECIOUS: $(simulator_app_deps)

# CAUTION: The empty line in this rule is important!
# Indeed, rule without receipe serve a special purpose
# https://www.gnu.org/software/make/manual/html_node/Canceling-Rules.html
.PRECIOUS: output/%.app
output/%.app: $(simulator_app_deps)
	$(Q)

# Standard usage: make -j8 PLATFORM=simulator TARGET=ios IOS_PROVISIONNING_PROFILE=build/artifacts/NumWorks_Graphing_Calculator_Development.mobileprovision IOS_SIGNER_IDENTITY="Apple Development: [DEVELOPER NAME]" epsilon.ipa

IOS_SIGNER_IDENTITY ?= Apple Distribution: NumWorks
output/%.ipa: output/%.app
ifdef IOS_PROVISIONNING_PROFILE
	$(call rule_label,SIGN)
	$(Q) codesign --force --entitlements output/app/entitlements.plist --sign "$(IOS_SIGNER_IDENTITY)" output/$*.app
endif
	@ echo "$(shell printf "%-8s" "COPY")$(notdir $*).app into Payload"
	$(Q) cd $(dir $@) ; mkdir Payload; cp -r $(notdir $*).app Payload

	$(call rule_label,ZIP)
	$(Q) cd $(dir $@) ; zip -qr9 $(notdir $@) Payload
