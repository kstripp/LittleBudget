if WANT_QT

bin_PROGRAMS = lbgui
lbgui_MOC = moc_choosefile.cpp \
	moc_filelistbox.cpp \
	moc_mainwindow.cpp
lbgui_UI = 
lbgui_LDADD := $(QT_LIBS) $(LDADD) ../core/liblb.la
lbgui_SOURCES = main.cpp \
	choosefile.cpp \
	choosefile.h \
	filelistbox.cpp \
	filelistbox.h \
	mainwindow.cpp \
	mainwindow.h
nodist_lbgui_SOURCES = $(lbgui_MOC) $(lbgui_UI)

AM_CXXFLAGS := $(CXXFLAGS) $(QT_CXXFLAGS)

BUILT_SOURCES = $(lbgui_MOC) $(lbgui_UI)
CLEANFILES = $(BUILT_SOURCES)

include ../../qtinclude.am

endif WANT_QT






