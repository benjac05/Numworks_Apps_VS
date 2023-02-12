TOOLCHAIN = apple
EXE = bin

APPLE_PLATFORM ?= ios
APPLE_PLATFORM_MIN_VERSION = 8.0
EPSILON_TELEMETRY ?= 1
TERMS_OF_USE ?= 0

ifeq ($(APPLE_PLATFORM),ios)
ARCHS = arm64 armv7
UI_REQUIRED_CAPABILITIES += armv7
else ifeq ($(APPLE_PLATFORM),ios-simulator)
ARCHS = x86_64
endif

BUILD_DIR := $(subst $(TARGET),$(APPLE_PLATFORM),output)

ifdef ARCH
BUILD_DIR := output/$(ARCH)
else
HANDY_TARGETS_EXTENSIONS = ipa app
endif
