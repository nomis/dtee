.PHONY: all debug compile debug-compile check clean debug-clean distclean install uninstall

RELEASE_DIR=build/release
DEBUG_DIR=build/debug
CC=clang
CXX=clang++

ifeq ($(V),1)
	NINJA=ninja -v
else
	NINJA=ninja
endif

all: compile
debug: debug-compile

build:
	mkdir -p build
$(RELEASE_DIR): build
	meson --buildtype=release $(RELEASE_DIR) || rm -rf "$(RELEASE_DIR)"
$(DEBUG_DIR): build
	meson --buildtype=debug $(DEBUG_DIR) || rm -rf "$(DEBUG_DIR)"

compile: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR)
debug-compile: $(DEBUG_DIR)
	$(NINJA) -C $(DEBUG_DIR)

check: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) cppcheck
	$(NINJA) -C $(RELEASE_DIR) clang-check
	$(NINJA) -C $(RELEASE_DIR) scan-build
	$(NINJA) -C $(RELEASE_DIR) test

clean: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) clean
debug-clean: $(DEBUG_DIR)
	$(NINJA) -C $(DEBUG_DIR) clean

distclean:
	rm -rf $(RELEASE_DIR) $(DEBUG_DIR)

install: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) install

uninstall: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) uninstall
