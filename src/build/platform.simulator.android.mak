TOOLCHAIN = android
EXE = so

EPSILON_TELEMETRY ?= 1
TERMS_OF_USE ?= 1

ARCHS = armeabi-v7a arm64-v8a x86 x86_64

ifdef ARCH
BUILD_DIR := output/$(ARCH)
else
HANDY_TARGETS_EXTENSIONS = apk
endif
