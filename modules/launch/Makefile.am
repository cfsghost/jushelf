module_LTLIBRARIES = launch.la
moduledir = $(libdir)/jushelf

launch_la_SOURCES = \
	launch.c

launch_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	$(PACKAGE_CFLAGS) \
	$(G_CAST_CHECKS)

launch_la_LDFLAGS = \
	-module \
	@JUSHELF_MODULE@
