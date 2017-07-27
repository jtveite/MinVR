# Create some test programs from the source files in this directory.

## Run these tests with 'make test' or 'ctest -VV' if you want to see
## the output.  You can also do 'ctest --memcheck' that runs the tests
## with some memory checking enabled.

set (displaytests projection)
# The numbers here correspond to 'case' statements in the respective
# test program.  See, e.g., datumtest.cpp
set (projection_parts 1 2 3)

# For tests where a list of parts has not been defined we add a default of 1:
foreach(displaytest ${displaytests})
  if(NOT DEFINED "${displaytest}_parts")
     set(${displaytest}_parts "1")
  endif()
endforeach()

# Don't forget the .cpp files for each test:
foreach(displaytest ${displaytests})
  set(displaytestsrc ${displaytestsrc} ${displaytest}test.cpp)
endforeach()

# Each of these .cpp files has a function with the same name as the
# file.

create_test_sourcelist(srclist RunSomeDisplayTests.cpp ${displaytestsrc})
add_executable(test-display ${srclist})
target_link_libraries(test-display MinVR)

# When it's compiled you can run the test-display executable and
# specify a particular test and subtest:
#./test-display queuetest 1
#All that's left is to tell CMake to generate the test cases:

set($ENV{MVRHOME} ${CMAKE_SOURCE_DIR})

foreach(displaytest ${displaytests})
  foreach(part ${${displaytest}_parts})
    add_test(NAME test_${displaytest}_${part}
      COMMAND ${CMAKE_BINARY_DIR}/bin/test-display ${displaytest}test ${part}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests-batch/config)
    set_tests_properties(test_${displaytest}_${part} PROPERTIES
      FAIL_REGULAR_EXPRESSION "ERROR;FAIL;Test failed")
  endforeach()
endforeach()
