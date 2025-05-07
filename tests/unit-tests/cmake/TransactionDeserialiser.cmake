################################################################################
# Module Description                                                           #
################################################################################

project(TransactionDeserialiser
  VERSION 1.2.0
  DESCRIPTION "Transaction deserialiser of the Solar app"
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

set(SOLAR_TRANSACTION_DESERIALISER_SRC
  "${SOLAR_CHECK_ASCII_SRC}"
  "${SOLAR_SOURCE_DIR}/transaction/deserialise_transaction.c"
  "${SOLAR_SOURCE_DIR}/transaction/types/ipfs.c"
  "${SOLAR_SOURCE_DIR}/transaction/types/transfer.c"
  "${SOLAR_SOURCE_DIR}/transaction/types/burn.c"
  "${SOLAR_SOURCE_DIR}/transaction/types/vote.c"
  "${SOLAR_SOURCE_DIR}/ui/ui_utils.c")

add_library(transaction_deserialiser "${LEDGER_LIB_STANDARD_SRC}" "${SOLAR_TRANSACTION_DESERIALISER_SRC}")

################################################################################
# Module Include Directories                                                   #
################################################################################

target_include_directories(transaction_deserialiser PUBLIC
  "$ENV{BOLOS_SDK}/include"
  "$ENV{BOLOS_SDK}/lib_standard_app"
  "${SOLAR_SOURCE_DIR}")
