FREETYPE ?= n

ifeq ($(FREETYPE),y)

#$(eval $(call pkg-config-library,FREETYPE,freetype2))
#FREETYPE_CPPFLAGS := $(patsubst -I%,-isystem %,$(FREETYPE_CPPFLAGS))
FREETYPE_CPPFLAGS := -I/home/joe/opt/freetype/include/freetype2

FREETYPE_CPPFLAGS += -DUSE_FREETYPE
LDFLAGS += -L/home/joe/opt/freetype/lib
LDLIBS += -l:/home/joe/opt/freetype/lib/libfreetype.a
endif
