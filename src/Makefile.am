## Process this file with automake to produce Makefile.in

bin_PROGRAMS = jushelf

INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-DPACKAGE_BIN_DIR=\""$(bindir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_LIB_DIR=\"$(libdir)\" \
	$(PACKAGE_CFLAGS) \
	$(G_CAST_CHECKS)

jushelf_SOURCES = \
	conf.h \
	conf.c \
	module.h \
	module.c \
	widget.h \
	widget.c \
	window.h \
	window.c \
	shelf.h \
	shelf.c \
	jushelf.h \
	jushelf.c

jushelf_LDADD = \
		$(PACKAGE_LIBS) \
		$(INTLLIBS)
