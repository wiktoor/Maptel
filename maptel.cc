#include <string>
#include <unordered_map>
#include "maptel.h"
using namespace std;

using dict = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dict>;

static directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

unsigned long maptel_create(void) {
    directory& dir = get_directory();
    static unsigned long gid = 0;

    dir.emplace(gid, dict());

    return gid++;
}

void maptel_delete(unsigned long id) {
    directory& dir = get_directory();
    dir.erase(id);
}



