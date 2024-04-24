
TESTS = ptr_test

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GENERICIMG_CFLAGS) \
	-I../src

AM_CFLAGS= \
	-Wall -Wpedantic

AM_CXXFLAGS = \
	-Wall -Wpedantic

check_LIBADD =  \
	$(GENERICIMG_LIBS)

check_PROGRAMS = ptr_test


ptr_test_SOURCES = \
    ptr_main.cpp

