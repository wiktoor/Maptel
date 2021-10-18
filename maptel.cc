#include <string>
#include <unordered_map>
#include "maptel.h"
using namespace std;

using dict = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dict>;
using id = unsigned long;

directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

unsigned long maptel_create(void) {
    directory& dir = get_directory();
    static id gid = 0;

    dir.emplace(gid, dict());

    return gid++;
}



