#include <string>
#include <vector>
#include <sstream>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

const long BASE = 10;

class BigInt {
public:
    std::vector<long> data;
    std::string str;

    BigInt(long n);

    char* to_str();

    void* add(BigInt& rhs);
    void* sub(BigInt& rhs);
    void* mul(BigInt& rhs);
    void* div(BigInt& rhs);
};

void insert_a_zero(std::vector<long>& num) {
    num.push_back(0);
}

void insert_zeros(std::vector<long>& num, int new_size) {
    while (num.size() < new_size)
        insert_a_zero(num);
}

void remove_zeros(std::vector<long>& num) {
    while (num.back() == 0)
        num.pop_back();
}

BigInt::BigInt(long n) {
    this->data = std::vector<long>({n});
}

char* BigInt::to_str() {
    std::stringstream str;
    for (int i = 0; i < this->data.size(); i++)
        str << this->data[i];
    this->str = str.str();
    return (char*)(void*)this->str.data();
}

void* BigInt::add(BigInt& rhs) {
    std::vector<long>* res = new std::vector<long>();
    reverse(this->data.begin(), this->data.end());
    reverse(rhs.data.begin(), rhs.data.end());
    int common_size = std::max(this->data.size(), rhs.data.size());
    insert_zeros(this->data, common_size);
    insert_zeros(rhs.data, common_size);
    for (int i = 0; i < common_size; i++) 
        res->push_back(this->data[i] + rhs.data[i]);
    insert_a_zero(*res);
    for (int i = 0 ;i < common_size; i++)
    {
        (*res)[i + 1] += (*res)[i] / BASE;
        (*res)[i] %= BASE;
    }
    remove_zeros(*res);
    reverse(res->begin(), res->end());
    return res;
}

void* BigInt::sub(BigInt& rhs) {
    std::vector<long>* res = new std::vector<long>();
    reverse(this->data.begin(), this->data.end());
    reverse(rhs.data.begin(), rhs.data.end());
    int common_size = std::max(this->data.size(), rhs.data.size());
    insert_zeros(this->data, common_size);
    insert_zeros(rhs.data, common_size);
    for (int i = 0; i < common_size; i++) 
        res->push_back(this->data[i] + rhs.data[i]);
    insert_a_zero(*res);
    for (int i = 0; i < common_size; i++)
    {
        if ((*res)[i] < 0)
        {
            (*res)[i] += BASE;
            (*res)[i + 1]--;
        }
    }
    
    remove_zeros(*res);
    reverse(res->begin(), res->end());
    return res;
}

void* BigInt::mul(BigInt& rhs) {
    int max_size = this->data.size() + rhs.data.size();

    std::vector<long>* res = new std::vector<long>(max_size);
    for (int i = this->data.size() - 1; i >= 0; i--) {
        for (int j = rhs.data.size() - 1; j >= 0; j--)
         (*res)[max_size - 2 - i - j] += this->data[i] * rhs.data[j];
    }
    
    for (int i=0; i<res->size() - 1;i++) {
        (*res)[i+1] += (*res)[i] / BASE;
        (*res)[i] %= BASE;
    }
    
    remove_zeros(*res);
    reverse(res->begin(), res->end());
    return res;
}

void* BigInt::div(BigInt& rhs) {
    return new BigInt(0);
}

extern "C" {
    void* new_bigint(long n) {
        return new BigInt(n);
    }

    void* copy_bigint(void* v) {
        auto b = new BigInt(0);
        b->data = ((BigInt*)v)->data;
        return b;
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
}
