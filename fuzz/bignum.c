#include <assert.h>
#include <stdio.h>
#include <openssl/bn.h>
#include <FuzzerInterface.h>

int LLVMFuzzerTestOneInput(const uint8_t *buf, size_t len) {
    int success = 1;
    static BN_CTX *ctx;
    static BN_MONT_CTX *mont;
    static BIGNUM *b1;
    static BIGNUM *b2;
    static BIGNUM *b3;
    static BIGNUM *b4;
    static BIGNUM *b5;
    if (ctx == NULL) {
        b1 = BN_new();
        b2 = BN_new();
        b3 = BN_new();
        b4 = BN_new();
        b5 = BN_new();
        ctx = BN_CTX_new();
        mont = BN_MONT_CTX_new();
    }
    size_t l1 = 0, l2 = 0, l3 = 0;
    int s1 = 0, s2 = 0, s3 = 0;
    if (len > 2) {
        l1 = (buf[0] * (len - 2)) / 255;
        ++buf;
        l2 = (buf[0] * (len - 2 - l1)) / 255;
        ++buf;
        l3 = len - 2 - l1 - l2;

        s1 = buf[0] & 1;
        s2 = buf[0] & 2;
        s3 = buf[0] & 4;
    }
    BN_bin2bn(buf, l1, b1);
    BN_set_negative(b1, s1);
    BN_bin2bn(buf + l1, l2, b2);
    BN_set_negative(b2, s2);
    BN_bin2bn(buf + l1 + l2, l3, b3);
    BN_set_negative(b3, s3);

    // mod 0 is undefined
    if (BN_is_zero(b3))
        goto done;

    assert(BN_mod_exp(b4, b1, b2, b3, ctx));
    assert(BN_mod_exp_simple(b5, b1, b2, b3, ctx));

    success = BN_cmp(b4, b5) == 0;
    if (!success) {
        BN_print_fp(stdout, b1);
        putchar('\n');
        BN_print_fp(stdout, b2);
        putchar('\n');
        BN_print_fp(stdout, b3);
        putchar('\n');
        BN_print_fp(stdout, b4);
        putchar('\n');
        BN_print_fp(stdout, b5);
        putchar('\n');
    }

 done:
    assert(success);

    return 0;
}