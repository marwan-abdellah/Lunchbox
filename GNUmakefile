#!gmake
<<<<<<< HEAD
.PHONY: debug release clean clobber package tests

ifeq ($(wildcard Makefile), Makefile)
all:
	@$(MAKE) -f Makefile $(MAKECMDGOALS)

clean:
	@$(MAKE) -f Makefile $(MAKECMDGOALS)

.DEFAULT:
	@$(MAKE) -f Makefile $(MAKECMDGOALS)

else

BUILD ?= Debug

normal: $(BUILD)/Makefile
	@$(MAKE) -C $(BUILD)

all: debug release
=======
.PHONY: debug release clean clobber package install debuginst releaseinst tests


ifeq ($(wildcard Makefile), Makefile)
all:
	$(MAKE) -f Makefile $(MAKECMDGOALS)

install:
	$(MAKE) -f Makefile $(MAKECMDGOALS)

clean:
	$(MAKE) -f Makefile $(MAKECMDGOALS)

.DEFAULT:
	$(MAKE) -f Makefile $(MAKECMDGOALS)

else

all: install
install: Debug/Makefile Release/Makefile
	@$(MAKE) -C Debug install
	@$(MAKE) -C Release install

>>>>>>> f5052001eecd5cfc7306d78c302586278b58461c
clean:
	@-$(MAKE) -C Debug clean
	@-$(MAKE) -C Release clean

<<<<<<< HEAD
packages: Release/Makefile
	@$(MAKE) -C Release packages

tests: $(BUILD)/Makefile
	@$(MAKE) -C $(BUILD) tests

.DEFAULT: $(BUILD)/Makefile
	@$(MAKE) -C $(BUILD) $(MAKECMDGOALS)
=======
package: Release/Makefile
	@$(MAKE) -C Release clean
	@$(MAKE) -C Release package

tests: Debug/Makefile
	@$(MAKE) -C Debug tests

.DEFAULT: Debug/Makefile
	@$(MAKE) -C Debug $(MAKECMDGOALS)
>>>>>>> f5052001eecd5cfc7306d78c302586278b58461c
endif

clobber:
	rm -rf Debug Release

debug: Debug/Makefile
	@$(MAKE) -C Debug

Debug/Makefile:
	@mkdir -p Debug
<<<<<<< HEAD
	@cd Debug; cmake .. -DCMAKE_BUILD_TYPE=Debug
=======
	@cd Debug; cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH=install
>>>>>>> f5052001eecd5cfc7306d78c302586278b58461c

release: Release/Makefile
	@$(MAKE) -C Release

Release/Makefile:
	@mkdir -p Release
<<<<<<< HEAD
	@cd Release; cmake .. -DCMAKE_BUILD_TYPE=Release

%-clean: $(BUILD)/%
	@$(MAKE) -C $< clean
=======
	@cd Release; cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=install

>>>>>>> f5052001eecd5cfc7306d78c302586278b58461c
