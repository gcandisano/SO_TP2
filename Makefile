
all:  bootloader kernel userland image installPreCommit

buddy:  bootloader kernelBuddy userland image installPreCommit

bootloader:
	cd Bootloader; make all

kernelBuddy:
	cd Kernel; make MM_MANAGER=BUDDY all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

installPreCommit: .pre-commit
	cp .pre-commit .git/hooks/pre-commit

.PHONY: bootloader image collections kernel userland all clean
