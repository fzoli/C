#############################################################################
# Makefile for building: dist/Release/GNU-Linux-x86/QtRadioInfo
# Generated by qmake (2.01a) (Qt 4.6.3) on: Sze �pr. 10 02:18:30 2013
# Project:  nbproject/qt-Release.pro
# Template: app
# Command: /usr/bin/qmake -unix VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -Inbproject -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. -Inbproject -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/local/lib -lcurlpp -lstdc++ -lcurl -lboost_regex -lboost_thread -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/Release/GNU-Linux-x86/

####### Files

SOURCES       = Music.cpp \
		RadioInfo.cpp \
		RadioInfoForm.cpp \
		main.cpp moc_RadioInfoForm.cpp
OBJECTS       = build/Release/GNU-Linux-x86/Music.o \
		build/Release/GNU-Linux-x86/RadioInfo.o \
		build/Release/GNU-Linux-x86/RadioInfoForm.o \
		build/Release/GNU-Linux-x86/main.o \
		build/Release/GNU-Linux-x86/moc_RadioInfoForm.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		nbproject/qt-Release.pro
QMAKE_TARGET  = QtRadioInfo
DESTDIR       = dist/Release/GNU-Linux-x86/
TARGET        = dist/Release/GNU-Linux-x86/QtRadioInfo

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: qttmp-Release.mk $(TARGET)

$(TARGET): ui_RadioInfoForm.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) dist/Release/GNU-Linux-x86/ || $(MKDIR) dist/Release/GNU-Linux-x86/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

qttmp-Release.mk: nbproject/qt-Release.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -unix VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix VPATH=. -o qttmp-Release.mk nbproject/qt-Release.pro

dist: 
	@$(CHK_DIR_EXISTS) nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0 || $(MKDIR) nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0/ && $(COPY_FILE) --parents Music.hpp RadioInfo.hpp RadioInfoForm.h nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0/ && $(COPY_FILE) --parents Music.cpp RadioInfo.cpp RadioInfoForm.cpp main.cpp nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0/ && $(COPY_FILE) --parents RadioInfoForm.ui nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0/ && (cd `dirname nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0` && $(TAR) QtRadioInfo1.0.0.tar QtRadioInfo1.0.0 && $(COMPRESS) QtRadioInfo1.0.0.tar) && $(MOVE) `dirname nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0`/QtRadioInfo1.0.0.tar.gz . && $(DEL_FILE) -r nbproject/build/Release/GNU-Linux-x86/QtRadioInfo1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) qttmp-Release.mk


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_RadioInfoForm.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_RadioInfoForm.cpp
moc_RadioInfoForm.cpp: Music.hpp \
		ui_RadioInfoForm.h \
		RadioInfoForm.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) RadioInfoForm.h -o moc_RadioInfoForm.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_RadioInfoForm.h
compiler_uic_clean:
	-$(DEL_FILE) ui_RadioInfoForm.h
ui_RadioInfoForm.h: RadioInfoForm.ui
	/usr/bin/uic-qt4 RadioInfoForm.ui -o ui_RadioInfoForm.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

build/Release/GNU-Linux-x86/Music.o: Music.cpp Music.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/Music.o Music.cpp

build/Release/GNU-Linux-x86/RadioInfo.o: RadioInfo.cpp RadioInfo.hpp \
		Music.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/RadioInfo.o RadioInfo.cpp

build/Release/GNU-Linux-x86/RadioInfoForm.o: RadioInfoForm.cpp RadioInfoForm.h \
		Music.hpp \
		ui_RadioInfoForm.h \
		RadioInfo.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/RadioInfoForm.o RadioInfoForm.cpp

build/Release/GNU-Linux-x86/main.o: main.cpp RadioInfoForm.h \
		Music.hpp \
		ui_RadioInfoForm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/main.o main.cpp

build/Release/GNU-Linux-x86/moc_RadioInfoForm.o: moc_RadioInfoForm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Release/GNU-Linux-x86/moc_RadioInfoForm.o moc_RadioInfoForm.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

