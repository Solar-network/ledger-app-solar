################################################################################
# Module Description                                                           #
################################################################################

project(Utils
  VERSION 1.2.0
  DESCRIPTION "Utilities of the Solar app"
  LANGUAGES C)

################################################################################
# Module Source Files                                                          #
################################################################################

set(SOLAR_CHECK_ASCII_SRC "${SOLAR_SOURCE_DIR}/utils/check_ascii.c")

set(SOLAR_UTILS_SRC "${SOLAR_CHECK_ASCII_SRC}")

add_library(utils "${SOLAR_UTILS_SRC}")

################################################################################
# Module Include Directories                                                   #
################################################################################

target_include_directories(utils PUBLIC "${SOLAR_SOURCE_DIR}")
