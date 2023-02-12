USE_LIBA = 0
EPSILON_GETOPT = 1

SFLAGS += -fPIE

TARGET ?= $(HOST)

COVERAGE =
ifneq ($(filter coverage,$(MAKECMDGOALS)),)
COVERAGE = coverage
endif

BUILD_DIR := output/$(TARGET)/$(COVERAGE)

include build/platform.simulator.$(TARGET).mak
