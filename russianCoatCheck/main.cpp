#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"

#include <algorithm>
#include <optional>
#include <vector>

template <class T>
class Registry
{
public:
    size_t Insert(T toInsert)
    {
        m_registry.emplace_back(m_nextKey, std::move(toInsert));
        auto result {m_nextKey};
        ++m_nextKey;
        ++m_size;
        return result;
    }

    std::optional<T> Get(size_t key) 
    {
        auto iter {FindKey(key)};
        if(m_registry.end() == iter)
        {
            return std::optional<T>();
        }
        if(key != iter->first)
        {
            return std::optional<T>();
        }
        return iter->second;
    }

    void Remove(size_t key)
    {
        auto iter {FindKey(key)};
        if(m_registry.end() == iter)
        {
            return;
        }
        iter->second.reset();
        --m_size;
        Compact();
    }
private:
    std::vector<std::pair<size_t, std::optional<T>>> m_registry;
    size_t m_nextKey = 0;
    size_t m_size = 0;

    using iterator = typename std::vector<std::pair<size_t, std::optional<T>>>::iterator;

    iterator FindKey(size_t key)
    {
        return std::lower_bound(m_registry.begin(), m_registry.end(), key, 
            [](const auto& a, const auto& b)
            {
                return a.first < b;
            });
    }

    void Compact()
    {
        if(m_size >= m_registry.size())
        {
            return;
        }
        m_registry.erase(std::remove_if(m_registry.begin(), m_registry.end(), 
            [](const auto& item)
            {
                return !item.second;
            }), 
            m_registry.end());
    };
};

TEST_CASE("two insertions")
{
    Registry<char> testObject;
    
    auto aKey {testObject.Insert('a')};
    REQUIRE(0 == aKey);

    auto bKey {testObject.Insert('b')};
    REQUIRE(1 == bKey);
}

TEST_CASE("simple get")
{
    Registry<char> testObject;
    
    auto aKey {testObject.Insert('a')};
    auto bKey {testObject.Insert('b')};

    auto aRetrieve {testObject.Get(aKey)};
    REQUIRE(aRetrieve);
    REQUIRE('a' == *aRetrieve);

    auto bRetrieve {testObject.Get(bKey)};
    REQUIRE(bRetrieve);
    REQUIRE('b' == *bRetrieve);
}

TEST_CASE("simple remove")
{
    Registry<char> testObject;
    
    auto aKey {testObject.Insert('a')};
    auto bKey {testObject.Insert('b')};

    testObject.Remove(aKey);
    testObject.Remove(bKey);
    auto aRetrieve {testObject.Get(aKey)};
    REQUIRE_FALSE(aRetrieve);

    auto bRetrieve {testObject.Get(bKey)};
    REQUIRE_FALSE(bRetrieve);
}

TEST_CASE("remove and get")
{
    Registry<char> testObject;
    
    auto aKey {testObject.Insert('a')};
    auto bKey {testObject.Insert('b')};
    auto cKey {testObject.Insert('c')};

    testObject.Remove(bKey);

    auto bFind {testObject.Get(bKey)};
    REQUIRE_FALSE(bFind);
}