# Generated automatically from Makefile.in by configure.
SHELL			=	/bin/sh

prefix			=	/home/cbilder
exec_prefix		=	${prefix}
host_os			=	linux-gnu
srcdir			=	.
top_srcdir		=	.
enable_debug		=	no

# Where to find includes for libraries that Bench depends on.
INCPATHS = -I$(prefix)/include  -I/usr/share/pvm3/include

# Where to install Bench's include files.
INCDEST = $(prefix)/include/BenchmarkLib

# Where to install Bench's library.
LIBDEST = $(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall
else
DEBUG= -O2
endif

CC= gcc
CXX= c++
CXXFLAGS= $(NOUCHAR) $(DEBUG) $(INCPATHS)
RANLIB=ranlib

SRCS=   Benchmark.cpp \
	PvmBenchmark.cpp \
	ServerBenchSlave.cpp \
	BenchmarkException.cpp \
	ServerBenchMaster.cpp	

OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libBenchmarkLib.a
	@echo "libBenchmarkLib.a successfully built."

libBenchmarkLib.a: $(OBJS)
	ar rsu libBenchmarkLib.a $(OBJS)

install: libBenchmarkLib.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libBenchmarkLib.a $(LIBDEST)

clean::
	rm -f libBenchmarkLib.a core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libBenchmarkLib.a

# Automatically rerun configure if the .in files have changed
#$(srcdir)/configure:	configure.in
#	cd $(srcdir) && autoconf

#$(srcdir)/stamp-h:  config.status
#	./config.status

#$(srcdir)/Makefile: Makefile.in config.status
#	./config.status

#$(srcdir)/config.status: configure
#	./config.status --recheck

