#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <gmp.h>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

struct __InternBig {
    void* num;
    long _rc;
};

class BigInt {
public:
    mpz_t num;

    BigInt(long n);
    BigInt(mpz_t n);
    ~BigInt();

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

BigInt::~BigInt() {
    mpz_clear(this->num);
}

char* BigInt::to_str() {
    return mpz_get_str(NULL, 10, this->num);
}

void* BigInt::add(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_add(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres), ._rc = 1};
}

void* BigInt::sub(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_sub(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres), ._rc = 1};
}

void* BigInt::mul(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_mul(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres), ._rc = 1};
}

void* BigInt::div(BigInt& rhs) {
    mpz_t nres;
    mpz_init(nres);
    mpz_div(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigInt(nres), ._rc = 1};
}



class BigFloat {
public:
    mpf_t num;
    std::string str;

    BigFloat(double n);
    BigFloat(double n, long precision);
    BigFloat(mpf_t n);
    ~BigFloat();

    char* to_str();

    void* add(BigFloat& rhs);
    void* sub(BigFloat& rhs);
    void* mul(BigFloat& rhs);
    void* div(BigFloat& rhs);
};

BigFloat::BigFloat(double n) {
    mpf_init_set_d(this->num, n);
}

BigFloat::BigFloat(double n, long precision) {
    mpf_init2(this->num, precision);
    mpf_set_d(this->num, n);
}

BigFloat::BigFloat(mpf_t n) {
    mpf_init_set(this->num, n);
}

BigFloat::~BigFloat() {
    mpf_clear(this->num);
}

char* BigFloat::to_str() {
    std::stringstream ss;
    ss << this->num;
    this->str = ss.str();
    return (char*)(void*)this->str.data();
}

void* BigFloat::add(BigFloat& rhs) {
    mpf_t nres;
    mpf_init(nres);
    mpf_add(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
}

void* BigFloat::sub(BigFloat& rhs) {
    mpf_t nres;
    mpf_init(nres);
    mpf_sub(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
}

void* BigFloat::mul(BigFloat& rhs) {
    mpf_t nres;
    mpf_init(nres);
    mpf_mul(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
}

void* BigFloat::div(BigFloat& rhs) {
    mpf_t nres;
    mpf_init(nres);
    mpf_div(nres, this->num, rhs.num);
    return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
}

extern "C" {
    void* new_bigint(long n) {
        return new BigInt(n);
    }

    void* copy_bigint(void* n) {
        return new __InternBig{.num = new BigInt(((BigInt*)n)->num), ._rc = 1};
    }

    char* big_to_str(void* b) {
        return ((BigInt*)b)->to_str();
    }

    void delete_bigint(void* b) {
        delete (BigInt*)b;
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
        return new __InternBig{.num = new BigInt(nres), ._rc = 1};
    }

    void* pow_bigint(void* b, long p) {
        mpz_t nres;
        mpz_init(nres);
        mpz_pow_ui(nres, ((BigInt*)b)->num, p);
        return new __InternBig{.num = new BigInt(nres), ._rc = 1};
    }

    void* neg_bigint(void* b) {
        mpz_t nres;
        mpz_init(nres);
        mpz_neg(nres, ((BigInt*)b)->num);
        return new __InternBig{.num = new BigInt(nres), ._rc = 1};
    }

    void* abs_bigint(void* b) {
        mpz_t nres;
        mpz_init(nres);
        mpz_abs(nres, ((BigInt*)b)->num);
        return new __InternBig{.num = new BigInt(nres), ._rc = 1};
    }

    long big_to_long(void* n) {
        return mpz_get_si(((BigInt*)n)->num);
    }

    long cmp_bigint(void* l, void* r) {
        return mpz_cmp(((BigInt*)l)->num, ((BigInt*)r)->num);
    }
    
    void* new_bigfloat(double n) {
        return new BigFloat(n);
    }

    void* new_bigfloat_prec(double n, long precision) {
        return new BigFloat(n, precision);
    }

    void* copy_bigfloat(void* n) {
        return new __InternBig{.num = new BigFloat(((BigFloat*)n)->num), ._rc = 1};
    }

    void delete_bigfloat(void* b) {
        delete (BigFloat*)b;
    }

    char* bigf_to_str(void* b) {
        return ((BigFloat*)b)->to_str();
    }

    double big_to_float(void* n) {
        return mpf_get_d(((BigFloat*)n)->num);
    }

    void* add_bigfloat(void* l, void* r) {
        BigFloat* lhs = (BigFloat*)l;
        BigFloat* rhs = (BigFloat*)r;
        return lhs->add(*rhs);
    }

    void* sub_bigfloat(void* l, void* r) {
        BigFloat* lhs = (BigFloat*)l;
        BigFloat* rhs = (BigFloat*)r;
        return lhs->sub(*rhs);
    }

    void* mul_bigfloat(void* l, void* r) {
        BigFloat* lhs = (BigFloat*)l;
        BigFloat* rhs = (BigFloat*)r;
        return lhs->mul(*rhs);
    }

    void* div_bigfloat(void* l, void* r) {
        BigFloat* lhs = (BigFloat*)l;
        BigFloat* rhs = (BigFloat*)r;
        return lhs->div(*rhs);
    }

    void* pow_bigfloat(void* b, long p) {
        mpf_t nres;
        mpf_init(nres);
        mpf_pow_ui(nres, ((BigFloat*)b)->num, p);
        return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
    }

    void* neg_bigfloat(void* b) {
        mpf_t nres;
        mpf_init(nres);
        mpf_neg(nres, ((BigFloat*)b)->num);
        return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
    }

    void* abs_bigfloat(void* b) {
        mpf_t nres;
        mpf_init(nres);
        mpf_abs(nres, ((BigFloat*)b)->num);
        return new __InternBig{.num = new BigFloat(nres), ._rc = 1};
    }

    long cmp_bigfloat(void* l, void* r) {
        return mpf_cmp(((BigFloat*)l)->num, ((BigFloat*)r)->num);
    }
}
