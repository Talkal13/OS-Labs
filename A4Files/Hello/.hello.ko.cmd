cmd_/home/talkal13/Documents/Projects/OS-Labs/A4Files/Hello/hello.ko := ld -r -m elf_x86_64 -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/talkal13/Documents/Projects/OS-Labs/A4Files/Hello/hello.ko /home/talkal13/Documents/Projects/OS-Labs/A4Files/Hello/hello.o /home/talkal13/Documents/Projects/OS-Labs/A4Files/Hello/hello.mod.o ;  true