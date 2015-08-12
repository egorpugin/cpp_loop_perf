#include <chrono>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#define GET_TIME(f) wrapper(#f, f)

using Vector = std::vector<int>;

template <typename F, typename ... Args>
auto get_time(const std::string &name, F &&f, Args && ... args)
{
    using namespace std::chrono;

    auto t0 = high_resolution_clock::now();
    f(args...);
    auto t1 = high_resolution_clock::now();
    return t1 - t0;
}

void tricky(Vector &vec)
{
    auto sz = vec.size();
    auto ptr = vec.data();
    while (sz--)
    {
        ++*ptr;
        ++ptr;
    }
}

void tricky_postfix_ptr(Vector &vec)
{
    auto sz = vec.size();
    auto ptr = vec.data();
    while (sz--)
        ++*ptr++;
}

void simple(Vector &vec)
{
    auto sz = vec.size();
    for (size_t i = 0; i < sz; ++i)
        ++vec[i];
}

void simple_slow_size(Vector &vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
        ++vec[i];
}

void simple_inc_postfix(Vector &vec)
{
    auto sz = vec.size();
    for (size_t i = 0; i < sz; i++)
        vec[i]++;
}

void simple_inc_postfix_slow_size(Vector &vec)
{
    for (size_t i = 0; i < vec.size(); i++)
        vec[i]++;
}

void foreach(Vector &vec)
{
    for (auto &v : vec)
        ++v;
}

void foreach_postfix(Vector &vec)
{
    for (auto &v : vec)
        v++;
}

void lambda_tricky_postfix_ptr(Vector &vec)
{
    auto inc = [](auto &v) { ++v; };
    auto sz = vec.size();
    auto ptr = vec.data();
    while (sz--)
        inc(*ptr++);
}

void lambda_simple(Vector &vec)
{
    auto inc = [](auto &v) { ++v; };
    auto sz = vec.size();
    for (size_t i = 0; i < sz; ++i)
        inc(vec[i]);
}

void lambda_in_loop_simple(Vector &vec)
{
    auto sz = vec.size();
    for (size_t i = 0; i < sz; ++i)
    {
        auto inc = [](auto &v) { ++v; };
        inc(vec[i]);
    }
}

void lambda_simple_capture(Vector &vec)
{
    auto c = rand();
    auto sz = vec.size();
    for (size_t i = 0; i < sz; ++i)
    {
        auto inc = [c = ++c](auto &v) { ++v; };
        inc(vec[i]);
    }
}

void lambda_simple_capture_foreach(Vector &vec)
{
    auto c = rand();
    for (auto &v : vec)
    {
        auto inc = [c = ++c](auto &v) { ++v; };
        inc(v);
    }
}

void lambda_foreach(Vector &vec)
{
    auto inc = [](auto &v) { ++v; };
    for (auto &v : vec)
        inc(v);
}

void iterator(Vector &vec)
{
    for (auto i = vec.begin(); i != vec.end(); ++i)
        ++*i;
}

template <class T>
void f_increment(T &v)
{
    v++;
}

void function(Vector &vec)
{
    auto sz = vec.size();
    for (size_t i = 0; i < sz; ++i)
        f_increment(vec[i]);
}

int main()
{
    srand((unsigned)time(0));

    const int n = 100'000'000;
    Vector vec(n);
    std::multimap<long long, std::string> results;

    auto wrapper = [&vec, &results, n](const auto &name, auto &&f, bool force_record = false)
    {
        using namespace std::chrono;

        auto v0 = vec[0];
        auto vN = vec[n - 1];
        auto time = get_time(name, f, vec);
        auto ms = duration_cast<milliseconds>(time).count();
        std::cout << name << ": ";
        if (v0 + 1 != vec[0] && vN + 1 != vec[n - 1] && !force_record)
        {
            std::cout << "operation did not increment the values! check sources";
        }
        else
        {
            results.insert(std::make_pair(ms, name));
            std::cout << ms << " ms.";
        }
        std::cout << "\n";
    };

    std::cout << "N = " << n << "\n\n";
    wrapper("create_vector_rand", [] (auto &vec) { for (auto &v : vec) v = rand(); }, true);
    GET_TIME(tricky);
    GET_TIME(tricky_postfix_ptr);
    GET_TIME(simple);
    GET_TIME(simple_slow_size);
    GET_TIME(simple_inc_postfix);
    GET_TIME(simple_inc_postfix_slow_size);
    GET_TIME(foreach);
    GET_TIME(foreach_postfix);
    GET_TIME(lambda_tricky_postfix_ptr);
    GET_TIME(lambda_simple);
    GET_TIME(lambda_in_loop_simple);
    GET_TIME(lambda_simple_capture);
    GET_TIME(lambda_foreach);
    GET_TIME(lambda_simple_capture_foreach);
    GET_TIME(iterator);
    GET_TIME(function);

    std::cout << "\n";
    std::cout << "sorted:\n";
    for (auto &r : results)
        std::cout << std::setw(6) << r.first << " : " << r.second << "\n";

    return 0;
}
