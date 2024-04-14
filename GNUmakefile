.PHONY: all debug compile debug-compile check coverage analyse clean distclean install uninstall pot po potfiles

BUILD_DIR=build
RELEASE_DIR=$(BUILD_DIR)/release
COVERAGE_DIR=$(BUILD_DIR)/coverage
DEBUG_DIR=$(BUILD_DIR)/debug

ifeq ($(V),1)
	NINJA=ninja -v
else
	NINJA=ninja
endif

ifeq ($(shell uname),Linux)
	D_B_LTO=-Db_lto=true
else
	D_B_LTO=
endif

ifeq ($(LTO),1)
	D_B_LTO=-Db_lto=true
else ifeq ($(LTO),0)
	D_B_LTO=-Db_lto=false
endif

ifeq ($(shell uname),OpenBSD)
	D_B_LUNDEF=-Db_lundef=false
	D_B_ASNEEDED=-Db_asneeded=false
else
	D_B_LUNDEF=
	D_B_ASNEEDED=
endif

all: compile
debug: debug-compile

$(BUILD_DIR)/:
	mkdir $(BUILD_DIR)/
$(RELEASE_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(RELEASE_DIR)/"
	mkdir $(RELEASE_DIR)/
	meson setup --buildtype=release $(RELEASE_DIR)/ $(D_B_LTO) $(D_B_LUNDEF) $(D_B_ASNEEDED) $(MESON_OPTS) || (rm -rf "$(RELEASE_DIR)/"; false)
$(COVERAGE_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(COVERAGE_DIR)/"
	mkdir $(COVERAGE_DIR)/
	meson setup --buildtype=debug $(COVERAGE_DIR)/ -Db_coverage=true $(MESON_OPTS) || (rm -rf "$(COVERAGE_DIR)/"; false)
$(DEBUG_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(DEBUG_DIR)/"
	mkdir $(DEBUG_DIR)/
	meson setup --buildtype=debug $(DEBUG_DIR)/ $(MESON_OPTS) || (rm -rf "$(DEBUG_DIR)/"; false)

compile: | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/
debug-compile: | $(DEBUG_DIR)/
	$(NINJA) -C $(DEBUG_DIR)/

check: | $(RELEASE_DIR)/
	rm -rf "$(RELEASE_DIR)/dtee@test/"
	$(NINJA) -C $(RELEASE_DIR)/ test

coverage: | $(COVERAGE_DIR)/
	$(NINJA) -C $(COVERAGE_DIR)/
	find "$(COVERAGE_DIR)/" -name '*.gcda' -exec rm {} \;
	rm -rf "$(COVERAGE_DIR)/dtee@test/"
	$(NINJA) -C $(COVERAGE_DIR)/ test
	rm -rf "$(COVERAGE_DIR)/meson-logs/coveragereport/"
	$(NINJA) -C $(COVERAGE_DIR)/ coverage-html

analyse: | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/ cppcheck
	$(NINJA) -C $(RELEASE_DIR)/ scan-build

clean:
ifneq ($(wildcard $(RELEASE_DIR)/),)
	$(NINJA) -C $(RELEASE_DIR)/ clean
endif
ifneq ($(wildcard $(COVERAGE_DIR)/),)
	$(NINJA) -C $(COVERAGE_DIR)/ clean
endif
ifneq ($(wildcard $(DEBUG_DIR)/),)
	$(NINJA) -C $(DEBUG_DIR)/ clean
endif

distclean:
	rm -rf $(BUILD_DIR)/

install: | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/ install

uninstall: | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/ uninstall

potfiles:
	ls -1 src/*.cpp >i18n/POTFILES

pot: potfiles | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/ dtee-pot

po:  potfiles | $(RELEASE_DIR)/
	$(NINJA) -C $(RELEASE_DIR)/ dtee-update-po
