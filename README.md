## Tuplex (simple tuple implementation with utils С++ 20)

## ⚙️ Available utils

- [X] tuple_cat
- [X] tuple_size_v
- [X] forward_as_tuple
- [X] make_tuple
- [X] get

## 📝 Code examples

```cpp
#include <iostream>

#include "headers/tuplex.hpp"
#include "headers/tuplex_utils.hpp"

int main()
{
    /* ----------------------------------------------------------------------------- */
    tuplex::tuple<int, float, std::string> tp{ 3, 5.5f, "hello world" }; 
    std::cout << tuplex::get<0>(tp) << '\n'; // -> 3

    /* ----------------------------------------------------------------------------- */
    tuplex::tuple<int, float> tp1{ 3, 5.5f };
    tuplex::tuple<std::string, std::string> tp2{ "hello", "world" };

    auto res{ tuplex::tuple_cat(tp1, tp2) }; // -> tuplex::tuple<int, float, std::string, std::string>

    /* ----------------------------------------------------------------------------- */
    tuplex::tuple<int, int, int> tp3{ 1, 2, 3 };
    std::cout << tuplex::tuple_size_v<decltype(tp3)> << '\n'; // -> 3

    return 0;
}
```