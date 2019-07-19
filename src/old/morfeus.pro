#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T20:56:01
#
#-------------------------------------------------

CONFIG += c++11 c++14
CONFIG -= qt
#CONFIG += staticlib

DEFINES += MORFEUS_COMPILE_LIBRARY

TARGET = morfeus
TEMPLATE = lib

DESTDIR = $$OUT_PWD/../lib
OBJECTS_DIR = $$OUT_PWD/obj
DLLDESTDIR = $$OUT_PWD/../bin

win32 {
  INCLUDEPATH += $$PWD/../morfeus/3rdparty/msmpi/Include
  contains(QMAKE_TARGET.arch, x86_64) {
    LIBS += -L$$PWD/3rdparty/msmpi/Lib/x64
    LIBS += -lmsmpi
  }
  else {
    LIBS += -L$$PWD/3rdparty/msmpi/Lib/x86
    LIBS += -lmsmpi
  }
}

SOURCES += \
    excitation/excitation.cpp \
    excitation/planewave.cpp \
    excitation/probefeed.cpp \
    material/isotropicmaterial.cpp \
    material/material.cpp \
    math/functions.cpp \
    math/guassquad.cpp \
    math/iterativesolver.cpp \
    mesh/edge.cpp \
    mesh/element.cpp \
    mesh/face.cpp \
    mesh/hexahedral.cpp \
    mesh/mesh.cpp \
    mesh/mesher.cpp \
    mesh/meshproperties.cpp \
    mesh/meshsection.cpp \
    mesh/node.cpp \
    mesh/prism.cpp \
    mesh/quadrilateral.cpp \
    mesh/triangle.cpp \
    observation/farfields.cpp \
    observation/inputimpedance.cpp \
    observation/observation.cpp \
    observation/sparameters.cpp \
    solver/iterativesolution.cpp \
    solver/solution.cpp \
    mesh/surfacemesh.cpp \
    mesh/layer.cpp

HEADERS += \
    excitation/excitation.h \
    excitation/planewave.h \
    excitation/probefeed.h \
    material/isotropicmaterial.h \
    material/material.h \
    math/constants.h \
    math/functions.h \
    math/gaussquad.h \
    math/iterativesolver.h \
    math/matrixvectormultiply.h \
    math/sparsematrix.h \
    math/uppertriangularmatrix.h \
    math/vector.h \
    mesh/edge.h \
    mesh/element.h \
    mesh/face.h \
    mesh/hexahedral.h \
    mesh/mesh.h \
    mesh/mesher.h \
    mesh/meshproperties.h \
    mesh/meshsection.h \
    mesh/node.h \
    mesh/prism.h \
    mesh/quadrilateral.h \
    mesh/triangle.h \
    observation/farfields.h \
    observation/inputimpedance.h \
    observation/observation.h \
    observation/sparameters.h \
    solver/iterativesolution.h \
    solver/solution.h \
    types.h \
    morfeus.h \
    mesh/surfacemesh.h \
    mesh/layer.h

