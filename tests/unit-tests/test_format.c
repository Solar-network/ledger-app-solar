#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "common/format.h"

static void test_format_i64(void **state) {
    (void) state;

    char temp[22] = {0};

    int64_t value = 0;
    assert_true(format_i64(temp, sizeof(temp), value));
    assert_string_equal(temp, "0");

    value = (int64_t) 9223372036854775807ull;  // MAX_INT64
    memset(temp, 0, sizeof(temp));
    assert_true(format_i64(temp, sizeof(temp), value));
    assert_string_equal(temp, "9223372036854775807");

    // buffer too small
    assert_false(format_i64(temp, sizeof(temp) - 5, value));

    value = (int64_t) -9223372036854775808ull;  // MIN_INT64
    memset(temp, 0, sizeof(temp));
    assert_true(format_i64(temp, sizeof(temp), value));
    assert_string_equal(temp, "-9223372036854775808");
}

static void test_format_u64(void **state) {
    (void) state;

    char temp[21] = {0};

    uint64_t value = 0;
    assert_true(format_u64(temp, sizeof(temp), value));
    assert_string_equal(temp, "0");

    value = (uint64_t) 18446744073709551615ull;  // MAX_UNT64
    memset(temp, 0, sizeof(temp));
    assert_true(format_u64(temp, sizeof(temp), value));
    assert_string_equal(temp, "18446744073709551615");

    // buffer too small
    assert_false(format_u64(temp, sizeof(temp) - 5, value));
}

static void test_format_fpu64(void **state) {
    (void) state;

    char temp[22] = {0};

    uint64_t amount = 100000000ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_fpu64(temp, sizeof(temp), amount, 8));
    assert_string_equal(temp, "1.00000000");

    amount = 24964823ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_fpu64(temp, sizeof(temp), amount, 8));
    assert_string_equal(temp, "0.24964823");

    amount = 100ull;
    memset(temp, 0, sizeof(temp));
    assert_true(format_fpu64(temp, sizeof(temp), amount, 8));
    assert_string_equal(temp, "0.00000100");

    amount = 10000000000000000000ull;
    assert_true(format_fpu64(temp, sizeof(temp), amount, 8));
    assert_string_equal(temp, "100000000000.00000000");

    // buffer too small
    assert_false(format_fpu64(temp, sizeof(temp) - 1, amount, 8));
}

static void test_format_amount(void **state) {
    (void) state;

    char temp[22 + 5] = {0};

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
    assert_false(format_amount(temp, sizeof(temp) - 1, amount, 8, "SXP", 3));
}

static void test_format_percentage(void **state) {
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

static void test_format_hex(void **state) {
    (void) state;

    uint8_t public_key[] = {0x03, 0xd3, 0x9f, 0xb4, 0x79, 0x7d, 0x0c, 0x42, 0x8b, 0xeb, 0xed,
                            0x6d, 0x80, 0x20, 0x3e, 0x22, 0x73, 0xa9, 0xfd, 0xbc, 0xaa, 0xfe,
                            0x0b, 0x29, 0x76, 0x1e, 0xf3, 0x18, 0x3c, 0x21, 0x51, 0xe2, 0x11};
    char output[2 * sizeof(public_key) + 1] = {0};

    assert_int_equal(2 * sizeof(public_key) + 1,
                     format_hex(public_key, sizeof(public_key), output, sizeof(output)));
    assert_string_equal(output,
                        "03d39fb4797d0c428bebed6d80203e2273a9fdbcaafe0b29761ef3183c2151e211");
    assert_int_equal(-1, format_hex(public_key, sizeof(public_key), output, sizeof(public_key)));
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_format_i64),
                                       cmocka_unit_test(test_format_u64),
                                       cmocka_unit_test(test_format_fpu64),
                                       cmocka_unit_test(test_format_hex),
                                       cmocka_unit_test(test_format_amount),
                                       cmocka_unit_test(test_format_percentage)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
