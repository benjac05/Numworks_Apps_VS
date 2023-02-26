SFLAGS += -Isrc/kandinsky/include
Q =

kandinsky_minimal_src += $(addprefix src/kandinsky/src/,\
  color.cpp \
  font.cpp \
  point.cpp \
  rect.cpp \
)


#REMOVED font.cpp

#kandinsky/src/color.cpp kandinsky/src/font.cpp kandinsky/src/point.cpp kandinsky/src/rect.cpp

kandinsky_src += $(addprefix src/kandinsky/src/,\
  color.cpp \
  context_line.cpp \
  context_pixel.cpp \
  context_rect.cpp \
  context_text.cpp \
  context_circle.cpp \
  font.cpp \
  framebuffer.cpp \
  ion_context.cpp \
  point.cpp \
  rect.cpp \
)

#REMOVED font.cpp

#see above for result

kandinsky_fonts_src += $(addprefix src/kandinsky/fonts/, \
  LargeFont.ttf \
  SmallFont.ttf \
)
kandinsky_src += output/kandinsky/fonts/codepoint_to_glyph_index.cpp

kandinsky_src += $(kandinsky_fonts_src)
kandinsky_minimal_src += $(kandinsky_fonts_src)

tests_src += $(addprefix src/kandinsky/test/,\
  color.cpp\
  rect.cpp\
)

define rule_for
ifneq ($(filter global,$(5)),)
$(addprefix output/,$(strip $(2))): $(strip $(3))
	@ echo "$(shell printf "%-8s" $(strip $(1)))$$(@:output/%=%)"
	$(Q) $(4)
endif
ifneq ($(filter local,$(5)),)
$(addprefix output/,$(strip $(2))): $(addprefix output/,$(strip $(3)))
	@ echo "$(shell printf "%-8s" $(strip $(1)))$$(@:output/%=%)"
	$(Q) $(4)
endif
endef

#REMOVED font.cpp
code_points = kandinsky/fonts/code_points.h

RASTERIZER_CFLAGS := -std=c11 -I./src/ion/include $(shell pkg-config freetype2 --cflags)
RASTERIZER_LDFLAGS := $(shell pkg-config freetype2 --libs)
LDFLAGS = -lfreetype -llz4
#RASTERIZER_LDFLAGS := -llz4

HAS_LIBPNG := $(shell pkg-config libpng --exists && echo 1)
ifeq ($(HAS_LIBPNG),1)
  RASTERIZER_CFLAGS += $(shell pkg-config libpng --cflags) -DGENERATE_PNG=1
  RASTERIZER_LDFLAGS += $(shell pkg-config libpng --libs)
endif

$(eval $(call rule_for, \
  HOSTCC, \
  kandinsky/fonts/rasterizer, \
  src/kandinsky/fonts/rasterizer.c src/kandinsky/fonts/code_points.h $(addprefix src/ion/src/external/lz4/, lz4.c lz4hc.c), \
  $$(HOSTCC) $$(RASTERIZER_CFLAGS) $$(filter-out %.h,$$^) $$(RASTERIZER_LDFLAGS) -o $$@, \
  global \
))

RASTERIZER := output/kandinsky/fonts/rasterizer


# Define a rasterizing recipe. Parameters : font name, size, packed_width, packed_height
define raster_font
$(call rule_for, \
  RASTER, \
  kandinsky/fonts/$(1).cpp, \
  kandinsky/fonts/$(1).ttf $$(RASTERIZER), \
  $$(RASTERIZER) $$< $(2) $(2) $(3) $(4) $(1) $$(dir $$@)/$(1).h $$@ $$(dir $$@)/codepoint_to_glyph_index.cpp $(if $(HAS_LIBPNG),$$(basename $$@).png), \
  global \
)
# codepoint_to_glyph_index.cpp is generated as an aside by the rasterizer
# we tell make that it can be built with an empty recipe as long as the font is built
output/kandinsky/fonts/codepoint_to_glyph_index.cpp: output/kandinsky/fonts/$(1).cpp
output/kandinsky/fonts/$(1).h: output/kandinsky/fonts/$(1).cpp
kandinsky_deps += output/kandinsky/fonts/$(1).h
endef

$(eval $(call raster_font,SmallFont,12,7,14))
$(eval $(call raster_font,LargeFont,16,10,18))

