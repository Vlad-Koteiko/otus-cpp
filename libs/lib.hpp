#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

using Pool = std::vector<std::string>;

int version();

void printPool(Pool &pool, std::ostream& file); 
void dinamikBlok(Pool &pool, std::ostream& file);
