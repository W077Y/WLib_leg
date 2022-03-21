#include <WLib_Utility.hpp>
#include <ut_catch.hpp>
#include <utility>    // for std::move

class T: WLib::Utility::non_copyable_non_moveable_t
{
  //...
};

TEST_CASE()
{
  [[maybe_unused]]T a;          // OK;
  [[maybe_unused]]T b = T();    // OK;
  // T c(a);	// FAIL no copy construct;
  // T c = a;	// FAIL no copy assign;
  // T c(std::move(a));		// FAIL no move construct;
  // T c = std::move(a);	// FAIL no move assign;
}
