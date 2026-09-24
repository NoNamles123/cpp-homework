# CMake generated Testfile for 
# Source directory: E:/Itstep - P51/C++/N91 Disk Cleaner
# Build directory: E:/Itstep - P51/C++/N91 Disk Cleaner/build-verify-20260904
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[N91DiskCleaner.CleanupLogic]=] "E:/Itstep - P51/C++/N91 Disk Cleaner/build-verify-20260904/Debug/N91DiskCleanerLogicTests.exe")
  set_tests_properties([=[N91DiskCleaner.CleanupLogic]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;59;add_test;E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[N91DiskCleaner.CleanupLogic]=] "E:/Itstep - P51/C++/N91 Disk Cleaner/build-verify-20260904/Release/N91DiskCleanerLogicTests.exe")
  set_tests_properties([=[N91DiskCleaner.CleanupLogic]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;59;add_test;E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[N91DiskCleaner.CleanupLogic]=] "E:/Itstep - P51/C++/N91 Disk Cleaner/build-verify-20260904/MinSizeRel/N91DiskCleanerLogicTests.exe")
  set_tests_properties([=[N91DiskCleaner.CleanupLogic]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;59;add_test;E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[N91DiskCleaner.CleanupLogic]=] "E:/Itstep - P51/C++/N91 Disk Cleaner/build-verify-20260904/RelWithDebInfo/N91DiskCleanerLogicTests.exe")
  set_tests_properties([=[N91DiskCleaner.CleanupLogic]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;59;add_test;E:/Itstep - P51/C++/N91 Disk Cleaner/CMakeLists.txt;0;")
else()
  add_test([=[N91DiskCleaner.CleanupLogic]=] NOT_AVAILABLE)
endif()
