# Path vars

PRJ_NAME	= mqtt-cpp

BUILDPATH	?= ..
BUILDPREFIX	?= build-

BUILDDIR 	= $(BUILDPATH)/$(BUILDPREFIX)$(PRJ_NAME)
OBJDIR 		= $(BUILDDIR)/obj
SRCDIR 		= src
APPDIR 		= app
EXECUTABLE 	= cpp-mqtt

HDRSRC	:= $(SRCDIR)/helper/default_params.hpp
APPSRC 	:= $(shell find $(APPDIR) -name '*.cpp')
SOURCES := $(shell find $(SRCDIR) -name '*.cpp') $(APPSRC) $(HDRSRC)
SRCDIRS := $(shell find . -not -path 'test' -name '*.cpp' -exec dirname {} \; | uniq)
OBJS 	:= $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

# -----------------------------------------------------------------------------
# Variables for testing
BUILDDIR_TEST	= $(BUILDDIR)
OBJDIR_TEST		= $(BUILDDIR)/obj-test
TESTDIR			= test
EXEC_SERVER		= cpp-mqtt-server

TESTSRC				:= $(shell find $(TESTDIR) -not -path 'server' -name '*.cpp')
TESTOBJS 			:= $(patsubst %.cpp,$(OBJDIR_TEST)/%.o,$(TESTSRC))
DIR_EXEC_SERVER		:= $(TESTDIR)/server
SRC_EXEC_SERVER		:= $(shell find $(DIR_EXEC_SERVER) -name '*.cpp') \
	$(shell find $(SRCDIR) -name '*.cpp' -exec dirname {} \; | uniq)
OBJS_EXEC_SERVER	:= $(patsubst %.cpp,$(OBJDIR_TEST)/%.0,$(DIR_EXEC_SERVER))
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Compiler and linker vars
INC 		+= -Isrc
CXXFLAGS 	+= -Wall -std=c++11
LDLIBS 		+= -lpaho-mqttpp3 -ljsoncpp

ifdef DEBUG
	CPPFLAGS += -DDEBUG
	CXXFLAGS += -g -O0
else
	CPPFLAGS += -D_NDEBUG
	CXXFLAGS += -O2
endif

#ifeq ($(DIST), "Ubuntu")
#	INC	+= -I/usr/include/jsoncpp
#endif
#ifeq ($(DIST), "Fedora")
	INC	+= -I/usr/include/jsoncpp
#endif
#ifeq ($(DIST), "openSUSE")
#	
#endif


# Compiler and linker vars for test
INC_TEST	+= $(INC) -Itest
# -----------------------------------------------------------------------------

.PHONY: clean distclean all

all: dir $(EXECUTABLE)

dir:
	@mkdir -p $(BUILDDIR)
	@for dir in $(patsubst ./%,%,$(SRCDIRS)); do \
		mkdir -p $(OBJDIR)/$$dir; \
	done

$(EXECUTABLE): $(BUILDDIR)/$(EXECUTABLE)

$(BUILDDIR)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $< -o $@
	
# -----------------------------------------------------------------------------
# Test
test: dir testdir $(EXEC_SERVER)

testdir:
	@mkdir -p $(BUILDDIR_TEST)
	@for dir in $(patsubst ./%,%,$(TESTSRC)); do \
		mkdir -p $(OBJDIR_TEST)/$$dir; \
	done
	
$(EXEC_SERVER): $(BUILDDIR_TEST)/$(EXEC_SERVER)

$(BUILDDIR_TEST)/$(EXEC_SERVER): $(OBJS_EXEC_SERVER)

$(OBJS_EXEC_SERVER)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $< -o $@
 
# -----------------------------------------------------------------------------	

# -----------------------------------------------------------------------------
# Cleanup
clean:
	rm -rf $(BUILDDIR)/*

distclean: clean
# -----------------------------------------------------------------------------