cmake_minimum_required(VERSION 3.20)
project(N91DiskCleaner VERSION 2.0.0 LANGUAGES CXX RC)

add_executable(N91DiskCleaner WIN32
    DiskCleaner.cpp
    resource.rc
    Localization.h
    LICENSE.txt
    README.txt
)

set_target_properties(N91DiskCleaner PROPERTIES
    OUTPUT_NAME "N91DiskCleaner"
)

target_compile_features(N91DiskCleaner PRIVATE cxx_std_17)
target_compile_definitions(N91DiskCleaner PRIVATE UNICODE _UNICODE)

if(MSVC)
    target_compile_options(N91DiskCleaner PRIVATE /utf-8 /W4 /EHsc)
    target_link_options(N91DiskCleaner PRIVATE /MANIFEST:NO)
    set_property(TARGET N91DiskCleaner PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_compile_options(N91DiskCleaner PRIVATE -Wall -Wextra)
endif()

target_link_libraries(N91DiskCleaner PRIVATE
    comctl32
    dwmapi
    shell32
    advapi32
    uxtheme
)

include(CTest)
if(BUILD_TESTING)
    add_executable(N91DiskCleanerLogicTests
        tests/CleanupLogicTests.cpp
    )
    target_compile_features(N91DiskCleanerLogicTests PRIVATE cxx_std_17)
    target_compile_definitions(N91DiskCleanerLogicTests PRIVATE
        UNICODE
        _UNICODE
        N91_DISK_CLEANER_LOGIC_TESTS
    )
    if(MSVC)
        target_compile_options(N91DiskCleanerLogicTests PRIVATE /utf-8 /W4 /EHsc)
        set_property(TARGET N91DiskCleanerLogicTests PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    target_link_libraries(N91DiskCleanerLogicTests PRIVATE
        comctl32
        dwmapi
        shell32
        advapi32
        uxtheme
    )
    add_test(NAME N91DiskCleaner.CleanupLogic COMMAND N91DiskCleanerLogicTests)
endif()
