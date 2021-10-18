#include <string>
#include <unordered_map>
#include "maptel.h"
using namespace std;

using dictionary = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dictionary>;

static directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

unsigned long maptel_create(void) {
    directory& dir = get_directory();
    static unsigned long gid = 0;

    dir.emplace(gid, dictionary());
    return gid++;
}

void maptel_delete(unsigned long id) {
    directory& dir = get_directory();
    dir.erase(id);
}

void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
    if (tel_src == NULL || tel_dst == NULL)
        return;

    dictionary& dict = get_directory()[id];
    dict.emplace(tel_src, tel_dst);
}

void maptel_erase(unsigned long id, char const *tel_src) {
    if (tel_src == NULL)
        return;

    dictionary& dict = get_directory()[id];
    dict.erase(tel_src);
}
