cmake_minimum_required(VERSION 3.22)
project(blinky C)

# C stuff
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(EXECUTABLE ${PROJECT_NAME}.out)
add_executable(${EXECUTABLE}
        Startup/startup_stm32f103xb.s
        Core/Src/system_stm32f1xx.c
        Core/Src/printf_redirect.c

        Peripherals/Src/rcc.c
        Peripherals/Src/uart.c
        Peripherals/Src/gpio.c
        main.c
)

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DSTM32F103xB
        -DDEBUG # Test macro
)

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        CMSIS/Include
        Peripherals/Inc
)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m3
        -mthumb

        -fdata-sections
        -ffunction-sections

        -Wall
        -Wextra
        -pedantic

        $<$<COMPILE_LANGUAGE:CXX>:@${CMAKE_SOURCE_DIR}/gcc-options-cxx.txt> # add C++ options (including the two above)

        $<$<CONFIG:Debug>:-Og> #  If CONFIG is Debug, then add option -Og, otherwise add nothing
)

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld
        -mcpu=cortex-m3
        -mthumb

        -specs=nano.specs
        -specs=nosys.specs # added to fix _sbrk not found issue
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections

        #-u _printf_float
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)

# Improve clean target
set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
        "${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map")