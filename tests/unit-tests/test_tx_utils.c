#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "format.h"

#include "transaction/transaction_utils.h"
#include "transaction/types.h"

static void test_tx_utils_format_amount(void **state) {
    (void) state;

    char temp[23 + 5] = {0};

    uint64_t amount = 100000000ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_amount(temp, sizeof(temp), amount, 8, "SXP", 3));

    assert_string_equal(temp, "1.00 SXP");

    amount = 24964823ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_amount(temp, sizeof(temp), amount, 8, "SXP", 3));
    assert_string_equal(temp, "0.24964823 SXP");

    amount = 100ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_amount(temp, sizeof(temp), amount, 8, "SXP", 3));
    assert_string_equal(temp, "0.000001 SXP");

    amount = 10000000000000000000ull;
    assert_true(format_amount(temp, sizeof(temp), amount, 8, "SXP", 3));
    assert_string_equal(temp, "100000000000.00 SXP");

    // buffer too small
    assert_false(format_amount(temp, sizeof(temp) - 2, amount, 8, "SXP", 3));
}

static void test_tx_utils_format_percentage(void **state) {
    (void) state;

    char temp[9] = {0};

    uint16_t amount = 10000ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_percentage(temp, sizeof(temp), amount, 2));
    assert_string_equal(temp, "100.00%");

    amount = 0ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_percentage(temp, sizeof(temp), amount, 2));
    assert_string_equal(temp, "0.00%");

    amount = 1234ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_percentage(temp, sizeof(temp), amount, 2));
    assert_string_equal(temp, "12.34%");

    amount = 22ull;
    assert_true(format_percentage(temp, sizeof(temp), amount, 2));
    assert_string_equal(temp, "0.22%");

    // buffer too small
    amount = 100ull;
    assert_false(format_percentage(temp, sizeof(temp) - 1, amount, 2));
}

static void test_tx_utils_check_ascii(void **state) {
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
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_tx_utils_format_amount),
                                       cmocka_unit_test(test_tx_utils_format_percentage),
                                       cmocka_unit_test(test_tx_utils_check_ascii)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
