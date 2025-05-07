################################################################################
# Module Description                                                           #
################################################################################

project(UiUtils
  VERSION 1.2.0
  DESCRIPTION "UI utils of the Solar app"
  LANGUAGES C)

################################################################################
# Module Source Files                                                          #
################################################################################

set(LEDGER_LIB_STANDARD_SRC "$ENV{BOLOS_SDK}/lib_standard_app/format.c")

set(SOLAR_UI_UTILS_SRC "${SOLAR_SOURCE_DIR}/ui/ui_utils.c")

add_library(ui_utils "${LEDGER_LIB_STANDARD_SRC}" "${SOLAR_UI_UTILS_SRC}")

################################################################################
# strlcat Shim                                                                 #
################################################################################
# Detects if strlcat() is available in the host system's standard library.
# If not present (common in non-BSD environments like older glibc), a
# compatible implementation is added to ensure test harness compatibility
# without modifying production code.

include(CheckSymbolExists)
check_symbol_exists(strlcat "string.h" HAVE_STRLCAT)

if (NOT HAVE_STRLCAT)
  message(STATUS "strlcat() not found in <string.h> → adding shim")
  target_sources(ui_utils PRIVATE
    "${SOLAR_UNIT_TEST_SRC_DIR}/strlcat_shim.c"
  )
endif()

################################################################################
# Module Include Directories                                                   #
################################################################################

target_include_directories(ui_utils PUBLIC
  "$ENV{BOLOS_SDK}/include"
  "$ENV{BOLOS_SDK}/lib_standard_app"
  "${SOLAR_SOURCE_DIR}")
