## Process this file with automake to produce Makefile.in

bin_PROGRAMS = jushelf

INCLUDES = \
	-I$(top_srcdir) \
	-DPACKAGE_BIN_DIR=\""$(bindir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	$(PACKAGE_CFLAGS) \
	$(G_CAST_CHECKS)

jushelf_SOURCES = \
	jushelf.h \
	jushelf.c

jushelf_LDADD = \
		$(PACKAGE_LIBS) \
		$(INTLLIBS)
