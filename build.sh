set -x #echo on
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/aes.d" -MT"build/aes.d" -Wa,-a,-ad,-alms=build/aes1.lst src/samtech/aes.c -o build/aes.o
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/cmac.d" -MT"build/cmac.d" -Wa,-a,-ad,-alms=build/cmac.lst src/samtech/cmac.c -o build/cmac.o
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/LoRaMacCrypto.d" -MT"build/LoRaMacCrypto.d" -Wa,-a,-ad,-alms=build/LoRaMacCrypto.lst src/samtech/LoRaMacCrypto.c -o build/LoRaMacCrypto.o
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/mini_fprint.d" -MT"build/mini_fprint.d" -Wa,-a,-ad,-alms=build/mini_fprint.lst src/mini_fprint.c -o build/mini_fprint.o
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/main.d" -MT"build/main.d" -Wa,-a,-ad,-alms=build/main.lst src/main.c -o build/main.o
arm-none-eabi-gcc -x assembler-with-cpp -c -mcpu=cortex-m0plus -mthumb -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/startup_stm32l072xx.d" -MT"build/startup_stm32l072xx.d" src/startup_stm32l072xx.s -o build/startup_stm32l072xx.o
arm-none-eabi-gcc build/mini_fprint.o build/startup_stm32l072xx.o build/aes.o build/cmac.o build/LoRaMacCrypto.o build/main.o -mcpu=cortex-m0plus -mthumb -specs=nano.specs -Tsrc/STM32L072CZEx_FLASH.ld  -lc -lm -lnosys -Wl,-Map=build/main.map,--cref -Wl,--gc-sections, -Wl,--print-memory-usage -o build/main.elf
arm-none-eabi-objcopy -O ihex build/main.elf build/main.hex
arm-none-eabi-objcopy -O binary -S build/main.elf build/main.bin
cp build/main.bin E:/
cp build/main.bin F:/