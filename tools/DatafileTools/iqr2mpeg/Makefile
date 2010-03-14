CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -O2 -g3 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -Wall -W -march=i586 -mcpu=i686 -fmessage-length=0 -Wall -fPIC 
CXXFLAGS = -pipe -O2 -g3 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -Wall -W -march=i586 -mcpu=i686 -fmessage-length=0 -Wall -fPIC 
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  =  -I. -I/usr/include  -I/usr/local/include/opencv
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS) -L/usr/lib/-L/usr/X11R6/lib/ -lm -lpopt -lcv -lcvaux -lhighgui
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = 
SOURCES = main.cpp
OBJECTS = main.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DESTDIR  = 
TARGET   = iqr2mpeg

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all:  $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)


$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )


dist: 
	@mkdir -p .tmp/streamParser && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/streamParser/ && ( cd `dirname .tmp/streamParser` && $(TAR) streamParser.tar streamParser && $(GZIP) streamParser.tar ) && $(MOVE) `dirname .tmp/streamParser`/streamParser.tar.gz . && $(DEL_FILE) -r .tmp/streamParser


yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

main.o: main.cpp 

####### Install

install:  

uninstall:  

