#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <gmp.h>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

struct __InternBig {
    void* num;
};

class BigInt {
public:
    mpz_t num;

    BigInt(long n);
    BigInt(mpz_t n);

    char* to_str();

    void* add(BigInt& rhs);
    void* sub(BigInt& rhs);
    void* mul(BigInt& rhs);
    void* div(BigInt& rhs);
};

BigInt::BigInt(long n) {
    mpz_init_set_si(this->num, n);
}

BigInt::BigInt(mpz_t n) {
    mpz_init_set(this->num, n);
}

char* BigInt::to_str() {
    return mpz_get_str(NULL, 10, this->num);
}

void* BigInt::add(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_add(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres)};
}

void* BigInt::sub(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_sub(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres)};
}

void* BigInt::mul(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_mul(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres)};
}

void* BigInt::div(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_div(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres)};
}

extern "C" {
    void* new_bigint(long n) {
        return new BigInt(n);
    }

    char* big_to_str(void* b) {
        return ((BigInt*)b)->to_str();
    }

    void* add_bigint(void* l, void* r) {
        BigInt* lhs = (BigInt*)l;
        BigInt* rhs = (BigInt*)r;
        return lhs->add(*rhs);
    }

    void* sub_bigint(void* l, void* r) {
        BigInt* lhs = (BigInt*)l;
        BigInt* rhs = (BigInt*)r;
        return lhs->sub(*rhs);
    }

    void* mul_bigint(void* l, void* r) {
        BigInt* lhs = (BigInt*)l;
        BigInt* rhs = (BigInt*)r;
        return lhs->mul(*rhs);
    }

    void* div_bigint(void* l, void* r) {
        BigInt* lhs = (BigInt*)l;
        BigInt* rhs = (BigInt*)r;
        return lhs->div(*rhs);
    }

    void* mod_bigint(void* l, void* r) {
        mpz_t nres;
        mpz_init(nres);
        mpz_mod(nres, ((BigInt*)l)->num, ((BigInt*)r)->num);
        return new __InternBig{.num = new BigInt(nres)};
    }

    void* pow_bigint(void* b, long p) {
        mpz_t nres;
        mpz_init(nres);
        mpz_pow_ui(nres, ((BigInt*)b)->num, p);
        return new __InternBig{.num = new BigInt(nres)};
    }

    void* neg_bigint(void* b) {
        mpz_t nres;
        mpz_init(nres);
        mpz_neg(nres, ((BigInt*)b)->num);
        return new __InternBig{.num = new BigInt(nres)};
    }

    void* abs_bigint(void* b) {
        mpz_t nres;
        mpz_init(nres);
        mpz_abs(nres, ((BigInt*)b)->num);
        return new __InternBig{.num = new BigInt(nres)};
    }

    long big_to_long(void* n) {
        return mpz_get_si(((BigInt*)n)->num);
    }

    long cmp_bigint(void* l, void* r) {
        return mpz_cmp(((BigInt*)l)->num, ((BigInt*)r)->num);
    }
}
