#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"
#include <tl/function_ref.hpp>

#include <functional>
#include <numeric>
#include <vector>

template<typename function, typename iterator>
void MutateTemplate(iterator begin, iterator end, function mutate)
{
   for(;begin != end; ++begin)
   {
      mutate(*begin);
   }
}

template<typename iterator>
void MutateStdFunction(iterator begin, iterator end, std::function<void(int&)> mutate)
{
   for(;begin != end; ++begin)
   {
      mutate(*begin);
   }
}

template<typename iterator>
void MutateFunctionRef(iterator begin, iterator end, const tl::function_ref<void(int&)>& mutate)
{
   for(;begin != end; ++begin)
   {
      mutate(*begin);
   }
}

std::vector<int> GetSimpleTestData(const size_t size)
{
   std::vector<int> result(size);
   std::iota(result.begin(), result.end(), 0);
   return result;
}

#define RunFunctionMacro(function, dataSetSize, numberOfCalls) \
{\
   for(size_t i {0}; i < numberOfCalls; ++i)\
   {\
      function(testData.begin(), testData.end(), \
         [](int& item)\
         {\
            item += 5;\
         });\
   }\
}

void RunTest(const size_t dataSetSize, const size_t numberOfCalls)
{
   auto testData {GetSimpleTestData(dataSetSize)};
   BENCHMARK("TemplateArg")
   {	
      RunFunctionMacro(MutateTemplate, dataSetSize, numberOfCalls);
   };

   testData = GetSimpleTestData(dataSetSize);
   BENCHMARK("StdFunction")
   {	
      RunFunctionMacro(MutateStdFunction, dataSetSize, numberOfCalls);
   };

   testData = GetSimpleTestData(dataSetSize);
   BENCHMARK("FunctionRef")
   {	
      RunFunctionMacro(MutateFunctionRef, dataSetSize, numberOfCalls);
   };
}

TEST_CASE("dataSetSize 1 numberOfCalls 100000000")
{
   constexpr const size_t dataSetSize {1};
   constexpr const size_t numberOfCalls {100'000'000};
   RunTest(dataSetSize, numberOfCalls);
}

TEST_CASE("dataSetSize 100000000 numberOfCalls 1")
{
   constexpr const size_t dataSetSize {100'000'000};
   constexpr const size_t numberOfCalls {1};
   RunTest(dataSetSize, numberOfCalls);
}