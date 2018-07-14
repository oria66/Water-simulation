TEMPLATE = app
TARGET = DEPTHWAVE

DEFINES -= UNICODE

CONFIG -= qt

win32 {
    LIBS *= user32.lib
    LIBS += -L$(OGRE_HOME)\\boost\\lib
    release:LIBS += -L$(OGRE_HOME)\\lib\\release
    debug:LIBS += -L$(OGRE_HOME)\\lib\\debug
    INCLUDEPATH += $(OGRE_HOME)\\include
    INCLUDEPATH += $(OGRE_HOME)\\include\\OIS
    INCLUDEPATH += $(OGRE_HOME)\\include\\OGRE
    INCLUDEPATH += $(OGRE_HOME)\\include\\OGRE\\Terrain
    INCLUDEPATH += $(OGRE_HOME)\\boost
   # INCLUDEPATH += $(OGRE_HOME)\\include\\Hydrax
    INCLUDEPATH += $(OGRE_HOME)\\include\\SKYX
    # If you are using Ogre 1.9 also include the following line:
     INCLUDEPATH += $(OGRE_HOME)\\include\\OGRE\\Overlay
    # INCLUDEPATH +=$(OGRE_HOME)\\cegui-0.8.7\\cegui\\include\\CEGUI
}
debug {
    TARGET = $$join(TARGET,,,d)
  # LIBS *= -lOgreMain_d -lOIS_d
   # If you are using Ogre 1.9 also include -lOgreOverlay_d, like this:
   LIBS *= -lOgreMain_d -lOIS_d -lOgreOverlay_d
}
release {
 TARGET = $$join(TARGET)
    #LIBS *= -lOgreMain -lOIS
    # If you are using Ogre 1.9 also include -lOgreOverlay, like this:
     LIBS *= -lOgreMain -lOIS -lOgreOverlay  -lSkyX # -lHydraX
# -lOgreTerrain

}

HEADERS += \
    DecalsManager.h \
    Enums.h \
    FFT.h \
    GodRaysManager.h \
    GPUNormalMapManager.h \
    Help.h \
    Hydrax.h \
    Image.h \
    MaterialManager.h \
    Mesh.h \
    Module.h \
    Noise.h \
    Perlin.h \
    Prerequisites.h \
    ProjectedGrid.h \
    RadialGrid.h \
    RttManager.h \
    SimpleGrid.h \
    TextureManager.h \
    depthwave.h


SOURCES += \
    DecalsManager.cpp \
    Enums.cpp \
    FFT.cpp \
    GodRaysManager.cpp \
    GPUNormalMapManager.cpp \
    Help.cpp \
    Hydrax.cpp \
    Image.cpp \
    MaterialManager.cpp \
    Mesh.cpp \
    Module.cpp \
    Noise.cpp \
    Perlin.cpp \
    Prerequisites.cpp \
    ProjectedGrid.cpp \
    RadialGrid.cpp \
    RttManager.cpp \
    SimpleGrid.cpp \
    TextureManager.cpp \
    depthwave.cpp

RC_FILE +=DEPTHWAVE.rc
