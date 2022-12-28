#include <iostream>
#include <vector>
#include <map>

using vector_int_type = std::vector<int>;

std::ostream& operator << (std::ostream& os, const vector_int_type& vect) {
    for (const auto& i : vect)
        os << '\t' << i;
    return os;
}

int print()
{
    std::map<std::string, vector_int_type> int_map;
    int_map[1] = vector_int_type{ 1,2,3 };
    int_map[2] = vector_int_type{ 4,5,6 };

    for (auto& item : int_map)
        std::cout << item.first << " is: " << item.second << std::endl;
}