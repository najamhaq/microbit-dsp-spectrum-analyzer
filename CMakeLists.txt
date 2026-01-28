cmake_minimum_required(VERSION 3.20)
project(microbit_dsp_spectrum_analyzer LANGUAGES C CXX ASM)

# ----------------------------
# Options / Target names
# ----------------------------
set(TARGET_NAME dsp-analyzer CACHE STRING "Firmware target name (base name, without extension)")
set(ELF_TARGET "${TARGET_NAME}.elf")

option(BUILD_FIRMWARE "Build embedded firmware (ARM cross-compile)" ${CMAKE_CROSSCOMPILING})
option(BUILD_TESTS    "Build host unit tests" ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# ----------------------------
# Common flags (ARM / Cortex-M4F)
# ----------------------------
set(MB2_CPU_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
)

function(mb2_apply_firmware_flags tgt)
    target_compile_options(${tgt} PRIVATE
            ${MB2_CPU_FLAGS}
            -ffreestanding
            -fdata-sections -ffunction-sections
            -Wall -Wextra
            $<$<CONFIG:Debug>:-Og -g3>
            $<$<CONFIG:Release>:-O2>
    )

    target_compile_options(${tgt} PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
    )

    target_compile_definitions(${tgt} PRIVATE
            $<$<CONFIG:Release>:NDEBUG>
            NRF52833_XXAA
    )

    target_link_options(${tgt} PRIVATE
            ${MB2_CPU_FLAGS}
            -nostdlib
            -Wl,--gc-sections
            -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map
            -T ${SRC_DIR}/platform/linker.ld
            $<$<CONFIG:Release>:-Wl,--strip-debug>
    )
endfunction()

# =====================================================================
# Firmware (ARM)
# =====================================================================
if(BUILD_FIRMWARE)
    add_executable(${ELF_TARGET}
            ${SRC_DIR}/platform/startup.s
            ${SRC_DIR}/platform/handlers.c
            ${SRC_DIR}/platform/mem.c

            # minimal app (hello-world / startup)
            ${SRC_DIR}/app/main.cpp

            # FreeRTOS minimum set (add more later as needed)
            ${SRC_DIR}/rtos/hooks.c
            ${SRC_DIR}/rtos/heap_4.c
            third_party/freertos-kernel/list.c
            third_party/freertos-kernel/queue.c
            third_party/freertos-kernel/tasks.c
            third_party/freertos-kernel/portable/GCC/ARM_CM4F/port.c
            src/app/include/app_task.h
            src/app/include/audio_task.h
            src/app/audio_task.cpp
            src/drivers/include/audio_capture.h
            src/drivers/audio_capture.cpp
            src/drivers/include/led_matrix.h
            src/drivers/led_matrix.cpp
            src/app/screen_refresh.cpp
            src/app/include/screen_refresh.h
            src/drivers/led_matrix.cpp
    )

    target_include_directories(${ELF_TARGET} PRIVATE
            ${SRC_DIR}/include
            ${SRC_DIR}/platform/include
            ${SRC_DIR}/rtos/include
            ${SRC_DIR}/app/include
            ${SRC_DIR}/drivers/include

            third_party/freertos-kernel/include
            third_party/freertos-kernel/portable/GCC/ARM_CM4F

            # Keep only if your startup/headers rely on them
            third_party/freertos-kernel/portable/GCC/ARM_CM4F
            third_party/cmsis_core/Include
            third_party/hal_nordic/nrfx/mdk
            third_party/nrfx/bsp/stable/mdk
            third_party/nrfx
            third_party/nrfx/mdk
    )

    mb2_apply_firmware_flags(${ELF_TARGET})

    # ----------------------------
    # Deploy (pyOCD)
    # ----------------------------
    # Use the Windows Python launcher so deploy/debug works from PowerShell, Git Bash, MSYS2, CLion
    find_program(PY_LAUNCHER py REQUIRED)
    add_custom_target(deploy
            COMMAND ${PY_LAUNCHER} -m pyocd flash --target nrf52833 --erase sector $<TARGET_FILE:${ELF_TARGET}>
            COMMAND ${PY_LAUNCHER} -m pyocd reset --target nrf52833
            DEPENDS ${ELF_TARGET}
            USES_TERMINAL
            VERBATIM
    )

    # ----------------------------
    # Debug server (pyOCD gdbserver)
    # ----------------------------
    set(PYOCD_FREQ 200000 CACHE STRING "pyOCD SWD frequency in Hz")

    add_custom_target(debugserver
            COMMAND ${PY_LAUNCHER} -m pyocd gdbserver --target nrf52833 --frequency ${PYOCD_FREQ} --port 3333
            USES_TERMINAL
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

endif()

# =====================================================================
# Unit tests (Host) - minimal
# =====================================================================
if(BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
    include(CTest)
    enable_testing()

    add_library(unity STATIC
            third_party/unity/src/unity.c
    )
    target_include_directories(unity PUBLIC
            third_party/unity/src
    )

    # one basic test executable
    add_executable(unit_tests
            tests/test_main.c
    )
    target_link_libraries(unit_tests PRIVATE unity)
    target_compile_definitions(unit_tests PRIVATE UNIT_TESTING=1)
    target_compile_options(unit_tests PRIVATE -g -O0)

    add_test(NAME unit_tests COMMAND unit_tests)
endif()
