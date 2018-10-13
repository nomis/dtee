.PHONY: all debug compile debug-compile check analyse clean debug-clean distclean install uninstall

BUILD_DIR=build
RELEASE_DIR=$(BUILD_DIR)/release
DEBUG_DIR=$(BUILD_DIR)/debug
CC=clang
CXX=clang++

ifeq ($(V),1)
	NINJA=ninja -v
else
	NINJA=ninja
endif

all: compile
debug: debug-compile

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(RELEASE_DIR): $(BUILD_DIR)
	mkdir -p $(RELEASE_DIR)
	meson --buildtype=release $(RELEASE_DIR) || rm -rf "$(RELEASE_DIR)"
$(DEBUG_DIR): $(BUILD_DIR)
	mkdir -p $(DEBUG_DIR)
	meson --buildtype=debug $(DEBUG_DIR) || rm -rf "$(DEBUG_DIR)"

compile: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR)
debug-compile: $(DEBUG_DIR)
	$(NINJA) -C $(DEBUG_DIR)

check: $(RELEASE_DIR)
	rm -rf "$(RELEASE_DIR)/dtee@test/"
	rm -rf "$(RELEASE_DIR)/meson-logs/coveragereport/"
	find "$(RELEASE_DIR)" -name '*.gcda' -exec rm {} \;
	$(NINJA) -C $(RELEASE_DIR) test
	$(NINJA) -C $(RELEASE_DIR) coverage-html

analyse: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) cppcheck
	$(NINJA) -C $(RELEASE_DIR) scan-build

clean: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) clean
debug-clean: $(DEBUG_DIR)
	$(NINJA) -C $(DEBUG_DIR) clean

distclean:
	rm -rf $(BUILD_DIR)

install: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) install

uninstall: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) uninstall
