libdir                 = @libdir@
datarootdir            = @datarootdir@
datadir                = @datadir@
includedir             = @includedir@
system_includedir      = @SYSTEM_INCLUDEDIR@
system_libdir          = @SYSTEM_LIBDIR@
pkgconfigdir           = @PKGCONFIGDIR@

CFLAGS = -Wall -Wextra -Wformat=2 -std=gnu99

lib_LTLIBRARIES = libbulb.la

pkginclude_HEADERS = libbulb/iter.h libbulb/libbulb.h
libbulb_la_SOURCES  =	\
		libbulb/libbulb-group.c		\
		libbulb/libbulb-light.c		\
		libbulb/libbulb-socket.c
libbulb_la_LDFLAGS = -version-info 1:0:0 -export-symbols-regex '^libbulb_'

EXTRA_PROGRAMS = unit_tests
unit_tests_SOURCES = tests/group-test.c
unit_tests_LDADD   = libbulb.la

CLEANFILES = $(EXTRA_PROGRAMS)

check: unit_tests
	./unit_tests
