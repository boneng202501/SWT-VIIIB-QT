# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SWT_VIIIB_NEW_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SWT_VIIIB_NEW_autogen.dir\\ParseCache.txt"
  "SWT_VIIIB_NEW_autogen"
  )
endif()
