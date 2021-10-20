#include <string>
#include <unordered_map>
#include <iostream>
#include <cctype>
#include "maptel.h"
using namespace std;

#ifdef NDEBUG
    #define DEBUG(x) ;
#else
    #define DEBUG(x) clog << x << '\n'
#endif

using dictionary = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dictionary>;

static directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

static bool tel_is_correct(string_view tel) {
    if (tel.length() > jnp1::TEL_NUM_MAX_LEN)
        return false;

    for (char c : tel) 
        if (!isdigit(c))
            return false;

    return true;
}

namespace jnp1 {
    unsigned long maptel_create(void) {
        DEBUG("maptel: maptel_create()");

        directory& dir = get_directory();
        static unsigned long gid = 0;
        dir.emplace(gid, dictionary());

        DEBUG("maptel: maptel_create: new map id = " << gid);
        return gid++;
    }

    void maptel_delete(unsigned long id) {
        DEBUG("maptel: maptel_delete(" << id << ")");

        directory& dir = get_directory();
        size_t cnt = dir.erase(id);

        if (!cnt)
            DEBUG("maptel: maptel_delete: nothing to delete");
        else
            DEBUG("maptel: maptel_delete: map " << id << "deleted");
    }

    void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
        if (tel_src == nullptr || tel_dst == nullptr) {
            DEBUG("maptel: maptel_insert: NULL pointer passed, skipping");
            return;
        } else {
            DEBUG("maptel: maptel_insert("
                    << id << ", " << tel_src << ", " << tel_dst << ")");
        }

        string_view tel_src_str { tel_src };
        string_view tel_dst_str { tel_src };

        if (!tel_is_correct(tel_src_str) || !tel_is_correct(tel_dst_str)) {
            DEBUG("maptel: maptel_insert: telephone number not correct");
            return;
        }

        dictionary& dict = get_directory()[id];
        dict.emplace(tel_src, tel_dst);

        DEBUG("maptel: maptel_insert: inserted");
    }

    void maptel_erase(unsigned long id, char const *tel_src) {
        if (tel_src == nullptr) {
            DEBUG("maptel: maptel_erase: NULL pointer passed, skipping");
            return;
        } else {
            DEBUG("maptel: maptel_erase(" << id << ", " << tel_src << ")");
        }

        dictionary& dict = get_directory()[id];
        size_t cnt = dict.erase(tel_src);

        if (!cnt)
            DEBUG("maptel: maptel_erase: nothing to erase");
        else
            DEBUG("maptel: maptel_erase: erased");
    }
}
