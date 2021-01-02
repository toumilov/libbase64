
CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
INSTALL_HEADERS_DIR := /usr/local/include/
INSTALL_LIB_DIR := /usr/local/lib/
SHARED_LIB := libbase64.so
STATIC_LIB := libbase64.a
OBJ_FILE := base64.o
CC = g++
AR = ar

.PHONY: all static shared install uninstall test clean

all: static shared

static: $(OBJ_FILE)
	$(AR) rcs $(STATIC_LIB) $^

shared: $(OBJ_FILE)
	$(CC) $^ -shared -o $(SHARED_LIB)

install: shared
	@echo Copying headers
	cp $(CURRENT_DIR)inc/base64.hpp $(INSTALL_HEADERS_DIR)
	@echo Copying library
	cp $(CURRENT_DIR)$(SHARED_LIB) $(INSTALL_LIB_DIR)
	@echo Done

uninstall:
	rm $(INSTALL_HEADERS_DIR)base64.hpp
	rm $(INSTALL_LIB_DIR)$(SHARED_LIB)
	@echo Done

# requires cpputest
# apt install cpputest
test: static $(CURRENT_DIR)test/tests.cpp
	$(CC) -I $(CURRENT_DIR)inc -c $(CURRENT_DIR)test/tests.cpp
	$(CC) tests.o -L $(CURRENT_DIR) -l:$(STATIC_LIB) -lCppUTest -lCppUTestExt -o unittests
	@echo Running tests...
	@exec $(CURRENT_DIR)unittests -v

clean:
	rm -rf $(CURRENT_DIR)$(STATIC_LIB)
	rm -rf $(CURRENT_DIR)$(SHARED_LIB)
	rm -rf $(CURRENT_DIR)$(OBJ_FILE)
	rm -rf $(CURRENT_DIR)tests.o
	rm -rf $(CURRENT_DIR)unittests

base64.o: $(CURRENT_DIR)src/base64.cpp
	$(CC) -I $(CURRENT_DIR)inc -c -o $@ $<
