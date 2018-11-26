#include <iostream>
#include "Genetics.hpp"

int main() {
    Population g;
    g.readCities("../cities.txt");
    g.run();

    return 0;
}