NULL =
BASE_SUBDIRS = \
	access \
	audio_filter \
	audio_mixer \
	audio_output \
	codec \
	control \
	demux \
	gui \
	meta_engine \
	misc \
	notify \
	packetizer \
	services_discovery \
	stream_filter \
	text_renderer \
	video_chroma \
	video_filter \
	video_output \
	video_splitter \
	visualization
EXTRA_SUBDIRS = \
	access_output \
	mux \
	stream_out \
	mmx \
	sse2 \
	altivec \
	arm_neon \
	hw/vdpau \
	lua \
	$(NULL)

SUBDIRS = $(BASE_SUBDIRS)
DIST_SUBDIRS = $(BASE_SUBDIRS) $(EXTRA_SUBDIRS) .
if ENABLE_SOUT
SUBDIRS += access_output mux stream_out
endif
if HAVE_MMX
SUBDIRS += mmx
endif
if HAVE_SSE2
SUBDIRS += sse2
endif
if HAVE_ALTIVEC
SUBDIRS += altivec
endif
if HAVE_NEON
SUBDIRS += arm_neon
endif
if HAVE_VDPAU
SUBDIRS += hw/vdpau
endif
if BUILD_LUA
SUBDIRS += lua
endif
SUBDIRS += .

dist_noinst_SCRIPTS = genmf list.sh module.rc.in
dist_noinst_DATA = LIST

if HAVE_WIN32
module.rc: $(top_builddir)/modules/module.rc.in $(top_builddir)/config.status
	$(AM_V_GEN) cd "$(top_builddir)" && $(SHELL) ./config.status --file="modules/$@"
endif
