# PNG
PNG_VERSION := 1.5.17
PNG_URL := $(SF)/libpng/libpng-$(PNG_VERSION).tar.bz2

PKGS += png
ifeq ($(call need_pkg,"libpng"),)
PKGS_FOUND += png
endif

$(TARBALLS)/libpng-$(PNG_VERSION).tar.bz2:
	$(call download,$(PNG_URL))

.sum-png: libpng-$(PNG_VERSION).tar.bz2

png: libpng-$(PNG_VERSION).tar.bz2 .sum-png
	$(UNPACK)
	$(APPLY) $(SRC)/png/winrt.patch
	$(APPLY) $(SRC)/png/automake.patch
	$(MOVE)

DEPS_png = zlib $(DEPS_zlib)

.png: png
	$(RECONF)
	cd $< && $(HOSTVARS) ./configure $(HOSTCONF)
	cd $< && $(MAKE) install
	touch $@
