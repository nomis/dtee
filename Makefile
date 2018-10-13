.PHONY: all debug compile test-compile debug-compile check analyse clean distclean install uninstall

BUILD_DIR=build
RELEASE_DIR=$(BUILD_DIR)/release
TEST_DIR=$(BUILD_DIR)/test
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
$(TEST_DIR): $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	meson --buildtype=release $(TEST_DIR) -Db_coverage=true || rm -rf "$(TEST_DIR)"
$(DEBUG_DIR): $(BUILD_DIR)
	mkdir -p $(DEBUG_DIR)
	meson --buildtype=debug $(DEBUG_DIR) || rm -rf "$(DEBUG_DIR)"

compile: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR)
test-compile: $(TEST_DIR)
	$(NINJA) -C $(TEST_DIR)
debug-compile: $(DEBUG_DIR)
	$(NINJA) -C $(DEBUG_DIR)

check: test-compile
	rm -rf "$(TEST_DIR)/dtee@test/"
	rm -rf "$(TEST_DIR)/meson-logs/coveragereport/"
	find "$(TEST_DIR)" -name '*.gcda' -exec rm {} \;
	$(NINJA) -C $(TEST_DIR) test
	$(NINJA) -C $(TEST_DIR) coverage-html

analyse: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) cppcheck
	$(NINJA) -C $(RELEASE_DIR) scan-build

clean:
	[ -d $(RELEASE_DIR) ] && $(NINJA) -C $(RELEASE_DIR) clean
	[ -d $(TESTE_DIR) ] && $(NINJA) -C $(TEST_DIR) clean
	[ -d $(DEBUG_DIR) ] && $(NINJA) -C $(DEBUG_DIR) clean

distclean:
	rm -rf $(BUILD_DIR)

install: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) install

uninstall: $(RELEASE_DIR)
	$(NINJA) -C $(RELEASE_DIR) uninstall
