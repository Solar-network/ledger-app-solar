cmake_minimum_required(VERSION 3.10)

if(${CMAKE_VERSION} VERSION_LESS 3.10)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# project information
project(unit_tests
        VERSION 0.1
	      DESCRIPTION "Unit tests for Ledger Nano application"
        LANGUAGES C)


# guard against bad build-type strings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

include(CTest)
ENABLE_TESTING()

# specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -pedantic -g -O0 --coverage")

set(GCC_COVERAGE_LINK_FLAGS "--coverage -lgcov")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

add_compile_definitions(TEST)

add_definitions(-DNANO_PXLS_PER_LINE=114 -DIS_NANOS=1)

include_directories(../../src)

add_executable(test_base58 test_base58.c)
add_executable(test_bip32 test_bip32.c)
add_executable(test_buffer test_buffer.c)
add_executable(test_format test_format.c)
add_executable(test_write test_write.c)
add_executable(test_apdu_parser test_apdu_parser.c)
add_executable(test_tx_parser test_tx_parser.c)
add_executable(test_tx_utils test_tx_utils.c)

add_library(nano_char_table nano_char_table.c)

add_library(base58 SHARED ../../src/common/base58.c)
add_library(bip32 SHARED ../../src/common/bip32.c)
add_library(buffer SHARED ../../src/common/buffer.c)
add_library(read SHARED ../../src/common/read.c)
add_library(write SHARED ../../src/common/write.c)
add_library(format SHARED ../../src/common/format.c)
add_library(varint SHARED ../../src/common/varint.c)
add_library(apdu_parser SHARED ../../src/apdu/parser.c)
add_library(transaction_deserialise ../../src/transaction/deserialise.c)
add_library(transaction_utils ../../src/transaction/utils.c)
add_library(transaction_types_multi_signature_registration ../../src/transaction/types/multi_signature_registration.c)
add_library(transaction_types_ipfs ../../src/transaction/types/ipfs.c)
add_library(transaction_types_transfer ../../src/transaction/types/transfer.c)
add_library(transaction_types_htlc_lock ../../src/transaction/types/htlc_lock.c)
add_library(transaction_types_htlc_refund ../../src/transaction/types/htlc_refund.c)
add_library(transaction_types_htlc_claim ../../src/transaction/types/htlc_claim.c)
add_library(transaction_types_burn ../../src/transaction/types/burn.c)
add_library(transaction_types_vote ../../src/transaction/types/vote.c)


target_link_libraries(test_base58 PUBLIC cmocka gcov base58)
target_link_libraries(test_bip32 PUBLIC cmocka gcov bip32 read)
target_link_libraries(test_buffer PUBLIC cmocka gcov buffer bip32 varint write read)
target_link_libraries(test_format PUBLIC cmocka gcov format nano_char_table)
target_link_libraries(test_write PUBLIC cmocka gcov write)
target_link_libraries(test_apdu_parser PUBLIC cmocka gcov apdu_parser)
target_link_libraries(test_tx_parser PUBLIC
                      transaction_deserialise
                      buffer
                      varint
                      bip32
                      cmocka
                      gcov
                      write
                      read
                      transaction_types_burn
                      transaction_types_htlc_claim
                      transaction_types_htlc_lock
                      transaction_types_htlc_refund
                      transaction_types_ipfs
                      transaction_types_multi_signature_registration
                      transaction_types_transfer
                      transaction_types_vote
                      transaction_utils)
target_link_libraries(test_tx_utils PUBLIC
                      cmocka
                      gcov
                      transaction_utils)

add_test(test_base58 test_base58)
add_test(test_bip32 test_bip32)
add_test(test_buffer test_buffer)
add_test(test_format test_format)
add_test(test_write test_write)
add_test(test_apdu_parser test_apdu_parser)
add_test(test_tx_parser test_tx_parser)
add_test(test_tx_utils test_tx_utils)
