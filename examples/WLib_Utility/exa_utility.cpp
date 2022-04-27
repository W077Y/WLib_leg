#include <WLib_Utility.hpp>
#include <utility>    // for std::move

class T: WLib::Utility::non_copyable_non_moveable_t
{
  //...
};

int main()
{
  [[maybe_unused]] T a;          // OK;
  [[maybe_unused]] T b = T();    // OK;
  //[[maybe_unused]]  T c(a);	// FAIL no copy construct;
  //[[maybe_unused]]  T c = a;	// FAIL no copy assign;
  //[[maybe_unused]]  T c(std::move(a));		// FAIL no move construct;
  //[[maybe_unused]]  T c = std::move(a);	// FAIL no move assign;
  return 0;
}
