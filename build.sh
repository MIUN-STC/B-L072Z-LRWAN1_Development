set -x #echo on
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/main.d" -MT"build/main.d" -Wa,-a,-ad,-alms=build/main.lst -IInclude main.c -o build/main.o
arm-none-eabi-gcc -x assembler-with-cpp -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/startup_stm32l072xx.d" -MT"build/startup_stm32l072xx.d" startup_stm32l072xx.s -o build/startup_stm32l072xx.o
arm-none-eabi-gcc build/startup_stm32l072xx.o build/main.o -mcpu=cortex-m0plus -mthumb -specs=nano.specs -TSTM32L072CZEx_FLASH.ld  -lc -lm -lnosys -Wl,-Map=build/main.map,--cref -Wl,--gc-sections -o build/main.elf
arm-none-eabi-objcopy -O ihex build/main.elf build/main.hex
arm-none-eabi-objcopy -O binary -S build/main.elf build/main.bin
mv build/main.bin F:/