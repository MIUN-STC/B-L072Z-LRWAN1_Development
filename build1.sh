set -x #echo on
gprbuild default.gpr -v
arm-eabi-objcopy -O binary -S bin/main.elf bin/main.bin
cp bin/main.bin E:/