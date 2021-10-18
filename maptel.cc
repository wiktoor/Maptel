#include <string>
#include <unordered_map>
#include "maptel.h"
using namespace std;

using dict = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dict>;

directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

