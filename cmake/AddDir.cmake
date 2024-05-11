function(dev_add_dir _DIR)
  get_filename_component(_FULLPATH "${_DIR}" REALPATH)

  if(EXISTS "${_FULLPATH}")
    return()
  else()
    file(MAKE_DIRECTORY ${_FULLPATH})
  endif()
endfunction()
