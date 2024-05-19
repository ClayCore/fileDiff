include(cmake/CPM.cmake)

function(dev_deps_setup)
  # This is a workaround to prevent cmake from forcing a very odd target toolchain for clang
  # this was the only thing that actually allows this project to build and import OpenSSL
  # at the same time.
  #
  # don't ask me why, or how
  set(_OPENSSL_ROOT_PATHS PATHS "S:\\wplace\\sdk\\openssl\\x64")
  find_package(
    OpenSSL 3.1.2
    REQUIRED
  )
endfunction()
