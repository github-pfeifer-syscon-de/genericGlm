
TESTS = ptr_test buf_test

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GENERICIMG_CFLAGS) \
	-I $(top_srcdir)/src

AM_CFLAGS= \
	-Wall -Wpedantic

AM_CXXFLAGS = \
	-Wall -Wpedantic

check_PROGRAMS = ptr_test buf_test

ptr_test_LDADD =  \
	$(GENERICIMG_LIBS)

buf_test_LDADD = \
	-L../src/.libs -lgenericglm

ptr_test_SOURCES = \
    ptr_main.cpp

buf_test_SOURCES = \
    buf_main.cpp
