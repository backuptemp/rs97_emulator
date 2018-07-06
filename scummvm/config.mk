# -------- Generated by configure -----------

CXX := mipsel-linux-g++
CXXFLAGS :=   -g -msoft-float -mips32 -ansi -W -Wno-unused-parameter -Wno-empty-body -pedantic
LD := mipsel-linux-g++
LIBS +=  -lm  -lvorbisidec  -lz -L/usr/lib/x86_64-linux-gnu -lSDL
RANLIB := mipsel-linux-ranlib
STRIP := mipsel-linux-strip
AR := mipsel-linux-ar cru
AS := mipsel-linux-as
ASFLAGS := 
WINDRES := mipsel-linux-windres
WINDRESFLAGS := 
WIN32PATH=C:/scummvm
AOS4PATH=Games:ScummVM
STATICLIBPATH=/sw

BACKEND := dingux
MODULES +=  backends/platform/sdl backends/platform/dingux
MODULE_DIRS += 
EXEEXT := 
NASM := 
NASMFLAGS := 

prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
datarootdir = ${prefix}/share
datadir = ${datarootdir}/scummvm
mandir = ${datarootdir}/man
docdir = ${datarootdir}/doc/scummvm


HAVE_GCC = 1
# VERBOSE_BUILD = 1
# USE_MT32EMU = 1
USE_RGB_COLOR = 1
USE_SCALERS = 1
# USE_HQ_SCALERS = 1
USE_INDEO3 = 1
# USE_VORBIS = 1
USE_TREMOR = 1
# USE_FLAC = 1
# USE_MAD = 1
USE_ZLIB = 1
# USE_MPEG2 = 1
# USE_NASM = 1
USE_TRANSLATION = 1
HAVE_GCC3 = 1
CXX_UPDATE_DEP_FLAG = -MMD -MF "$(*D)/$(DEPDIR)/$(*F).d" -MQ "$@" -MP
ENABLE_SCUMM = STATIC_PLUGIN
ENABLE_SCUMM_7_8 = 1
ENABLE_HE = 1
ENABLE_AGI = STATIC_PLUGIN
ENABLE_AGOS = STATIC_PLUGIN
ENABLE_AGOS2 = 1
ENABLE_CINE = STATIC_PLUGIN
ENABLE_CRUISE = STATIC_PLUGIN
ENABLE_DRACI = STATIC_PLUGIN
ENABLE_DRASCULA = STATIC_PLUGIN
ENABLE_GOB = STATIC_PLUGIN
ENABLE_GROOVIE = STATIC_PLUGIN
# ENABLE_GROOVIE2
# ENABLE_HUGO
ENABLE_KYRA = STATIC_PLUGIN
# ENABLE_LOL
ENABLE_LURE = STATIC_PLUGIN
# ENABLE_M4
ENABLE_MADE = STATIC_PLUGIN
# ENABLE_MOHAWK
ENABLE_PARALLACTION = STATIC_PLUGIN
ENABLE_QUEEN = STATIC_PLUGIN
ENABLE_SAGA = STATIC_PLUGIN
ENABLE_IHNM = 1
# ENABLE_SAGA2
ENABLE_SCI = STATIC_PLUGIN
# ENABLE_SCI32
ENABLE_SKY = STATIC_PLUGIN
ENABLE_SWORD1 = STATIC_PLUGIN
ENABLE_SWORD2 = STATIC_PLUGIN
ENABLE_TEENAGENT = STATIC_PLUGIN
# ENABLE_TESTBED
ENABLE_TINSEL = STATIC_PLUGIN
ENABLE_TOUCHE = STATIC_PLUGIN
ENABLE_TUCKER = STATIC_PLUGIN

INCLUDES +=    -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
OBJS += 
DEFINES +=  -DUNIX -DDINGUX -DDISABLE_DOSBOX_OPL -DREDUCE_MEMORY_USAGE -DUNIX -DENABLE_VKEYBD -DDATA_PATH=\"${datarootdir}/scummvm\" -DPLUGIN_DIRECTORY=\"${exec_prefix}/lib/scummvm\" -DSDL_BACKEND -DDINGUX
LDFLAGS +=  



port_mk = backends/platform/dingux/dingux.mk

SAVED_CONFIGFLAGS  := --host=mipsel-linux
SAVED_LDFLAGS      := 
SAVED_CXX          := mipsel-linux-g++
SAVED_CXXFLAGS     := 
SAVED_CPPFLAGS     := 
SAVED_ASFLAGS      := 
SAVED_WINDRESFLAGS := 
