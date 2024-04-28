function(dev_check_libfuzzer_support var_name)
  set(
    libFuzzerTestSource
    "
#include <cstdint>
extern \"C\" int LLVMFuzzerTestOneInput(std::uint8_t const *data, std::size_t size) {
    return 0;
}"
  )

  include(CheckCXXSourceCompiles)

  set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
  set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=fuzzer")

  check_cxx_source_compiles("${libFuzzerTestSource}" ${var_name})
endfunction()
