/*#include <iostream>

#include "../include/utilities/csv.h"
#include "../tinyfiledialogs/tinyfiledialogs.h"

int main() {
    char const* lFilterPatterns[1] = { "*.csv" };

    char const* lTheOpenFileName = tinyfd_openFileDialog("Select a File", "", 1, lFilterPatterns, "(.csv) Comma Separated Values", 0);

    if (lTheOpenFileName) {
        CSV_T csv(lTheOpenFileName);

        csv.read();

        convert_result<int> year = csv.map()[24].convert<int>(2);

        std::cout << std::boolalpha;
        std::cout << csv.map()[24].at(2) << '\n';
        std::cout << year.success() << " " << year.value() << '\n';
    }
    return 0;
}*/