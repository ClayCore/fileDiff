function(dev_ensure_out_of_source_build)
  get_filename_component(sourceDir "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(binaryDir "${CMAKE_BINARY_DIR}" REALPATH)

  if("${sourceDir}" STREQUAL "${binaryDir}")
    message(SEND_ERROR " * in-source builds are explicitly disabled")
    message(SEND_ERROR " * create a separate build directory")
    message(FATAL_ERROR " * quitting configuration...")
  endif()
endfunction()

dev_ensure_out_of_source_build()
