.PHONY: all debug compile debug-compile check coverage analyse clean distclean install uninstall

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
	LTO=true
else
	LTO=false
endif

all: compile
debug: debug-compile

$(BUILD_DIR)/:
	mkdir $(BUILD_DIR)/
$(RELEASE_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(RELEASE_DIR)/"
	mkdir $(RELEASE_DIR)/
	meson --buildtype=release $(RELEASE_DIR)/ -Db_lto=$(LTO) || (rm -rf "$(RELEASE_DIR)/"; false)
$(COVERAGE_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(COVERAGE_DIR)/"
	mkdir $(COVERAGE_DIR)/
	# meson (0.48.0) will not tell lcov to use llvm-cov, so this will fail when using clang
	meson --buildtype=debug $(COVERAGE_DIR)/ -Db_coverage=true || (rm -rf "$(COVERAGE_DIR)/"; false)
$(DEBUG_DIR)/: | $(BUILD_DIR)/
	rm -rf "$(DEBUG_DIR)/"
	mkdir $(DEBUG_DIR)/
	meson --buildtype=debug $(DEBUG_DIR)/ || (rm -rf "$(DEBUG_DIR)/"; false)

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
