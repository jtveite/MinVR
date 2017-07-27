# Create some test programs from the source files in this directory.

## Run these tests with 'make test' or 'ctest -VV' if you want to see
## the output.  You can also do 'ctest --memcheck' that runs the tests
## with some memory checking enabled.

set (inputtests faketracker)
# The numbers here correspond to 'case' statements in the respective
# test program.  See, e.g., datumtest.cpp
set (faketracker_parts 1 2)

# For tests where a list of parts has not been defined we add a default of 1:
foreach(inputtest ${inputtests})
  if(NOT DEFINED "${inputtest}_parts")
     set(${inputtest}_parts "1")
  endif()
endforeach()

# Don't forget the .cpp files for each test:
foreach(inputtest ${inputtests})
  set(inputtestsrc ${inputtestsrc} ${inputtest}test.cpp)
endforeach()

# Each of these .cpp files has a function with the same name as the
# file.

create_test_sourcelist(srclist RunSomeInputTests.cpp ${inputtestsrc})
add_executable(test-input ${srclist})
target_link_libraries(test-input MinVR)

# When it's compiled you can run the test-input executable and
# specify a particular test and subtest:
#./test-input queuetest 1
#All that's left is to tell CMake to generate the test cases:

set($ENV{MVRHOME} ${CMAKE_SOURCE_DIR})

foreach(inputtest ${inputtests})
  foreach(part ${${inputtest}_parts})
    add_test(NAME test_${inputtest}_${part}
      COMMAND ${CMAKE_BINARY_DIR}/bin/test-input ${inputtest}test ${part}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests-batch/config)
    set_tests_properties(test_${inputtest}_${part} PROPERTIES
      FAIL_REGULAR_EXPRESSION "ERROR;FAIL;Test failed")
  endforeach()
endforeach()
