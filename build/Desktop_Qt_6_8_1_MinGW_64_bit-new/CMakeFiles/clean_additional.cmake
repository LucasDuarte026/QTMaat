# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QTMaat_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QTMaat_autogen.dir\\ParseCache.txt"
  "QTMaat_autogen"
  )
endif()
