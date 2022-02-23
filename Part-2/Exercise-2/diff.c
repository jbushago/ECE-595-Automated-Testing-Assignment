#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int16_t saturating_rounding_double_high_mul_a(int16_t a, int16_t b) {
    bool overflow = a == b && a == INT16_MIN;
    int32_t a_32 = a;
    int32_t b_32 = b;
    int32_t ab_32 = a_32 * b_32;
    int16_t nudge = ab_32 >= 0 ? (1 << 14) : (1 - (1 << 14));
    int16_t ab_x2_high16 = (int16_t) ((ab_32 + nudge) / (1 << 15));
    return overflow ? INT16_MAX : ab_x2_high16;
}

int16_t saturating_rounding_double_high_mul_b(int16_t a, int16_t b) {
    bool overflow = a == b && a == INT16_MIN;
    int32_t a_32 = a;
    int32_t b_32 = b;
    int32_t ab_32 = a_32 * b_32;
    int16_t nudge = ab_32 >= 0 ? (1 << 14) : (1 - (1 << 14));
    int16_t ab_x2_high16 = (int16_t) ((ab_32 + nudge) >> 15);
    return overflow ? INT16_MAX : ab_x2_high16;
}

int main() {
    /*
     * Read 2 `int16_t`s from `stdin` and `assert`
     * that implementations a and b return the same value when run
     * on the inputs.
     */
    return EXIT_SUCCESS;
}
