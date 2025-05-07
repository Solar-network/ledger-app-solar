################################################################################
# Module Description                                                           #
################################################################################

project(MessageDeserialiser
  VERSION 1.2.0
  DESCRIPTION "Message deserialiser of the Solar app"
  LANGUAGES C)

################################################################################
# Module Source Files                                                          #
################################################################################

set(LEDGER_LIB_STANDARD_SRC
  "$ENV{BOLOS_SDK}/lib_standard_app/buffer.c"
  "$ENV{BOLOS_SDK}/lib_standard_app/varint.c"
  "$ENV{BOLOS_SDK}/lib_standard_app/bip32.c"
  "$ENV{BOLOS_SDK}/lib_standard_app/write.c"
  "$ENV{BOLOS_SDK}/lib_standard_app/read.c")

set(SOLAR_CHECK_ASCII_SRC "${SOLAR_SOURCE_DIR}/utils/check_ascii.c")

set(SOLAR_MESSAGE_DESERIALISER_SRC
  "${SOLAR_CHECK_ASCII_SRC}"
  "${SOLAR_SOURCE_DIR}/transaction/deserialise_message.c")

add_library(message_deserialiser "${LEDGER_LIB_STANDARD_SRC}" "${SOLAR_MESSAGE_DESERIALISER_SRC}")

################################################################################
# Module Include Directories                                                   #
################################################################################

target_include_directories(message_deserialiser PUBLIC
  "$ENV{BOLOS_SDK}/include"
  "$ENV{BOLOS_SDK}/lib_standard_app"
  "${SOLAR_SOURCE_DIR}")
