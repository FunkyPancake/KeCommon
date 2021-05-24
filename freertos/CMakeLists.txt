cmake_minimum_required (VERSION 3.13)
project(freertos VERSION 1.0.0 LANGUAGES CXX C)

target_sources(KeCommon PRIVATE
        freertos_kernel/portable/MemMang/heap_4.c
        freertos_kernel/portable/GCC/ARM_CM4F/port.c
        freertos_kernel/croutine.c
        freertos_kernel/event_groups.c
        freertos_kernel/list.c
        freertos_kernel/queue.c
        freertos_kernel/stream_buffer.c
        freertos_kernel/tasks.c
        freertos_kernel/timers.c
        )
target_include_directories(KeCommon PUBLIC
        freertos_kernel
        freertos_kernel/include
        freertos_kernel/portable/GCC/ARM_CM4F
        freertos_kernel/portable/MemMang
        )
