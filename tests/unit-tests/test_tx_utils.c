#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "transaction/utils.h"
#include "transaction/types.h"

static void test_tx_utils(void **state) {
    (void) state;

    const uint8_t good_ascii[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21};                 // Hello!
    const uint8_t bad_ascii[] = {0x32, 0x00, 0x32, 0x3d, 0x34};                        // 2\02=4
    const uint8_t good_newline_ascii1[] = {0x63, 0x69, 0x61, 0x6F, 0x0a};              // ciao\n
    const uint8_t good_newline_ascii2[] = {0x32, 0x0a, 0x32, 0x3d, 0x34, 0x0d, 0x0a};  // ciao\r\n

    assert_true(transaction_utils_check_ascii(good_ascii, sizeof(good_ascii), false));
    assert_false(transaction_utils_check_ascii(bad_ascii, sizeof(bad_ascii), false));
    assert_false(
        transaction_utils_check_ascii(good_newline_ascii1, sizeof(good_newline_ascii1), false));
    assert_false(
        transaction_utils_check_ascii(good_newline_ascii2, sizeof(good_newline_ascii2), false));

    assert_true(transaction_utils_check_ascii(good_newline_ascii1, sizeof(good_ascii), true));
    assert_true(transaction_utils_check_ascii(good_newline_ascii1, sizeof(good_ascii), true));
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_tx_utils)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
