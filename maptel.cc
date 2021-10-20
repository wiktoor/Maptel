#include <string>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cctype>
#include "maptel.h"
using namespace std;

#ifdef NDEBUG
    #define DEBUG(x) do { } while(0)
#else
    #define DEBUG(x) do { cerr << x << '\n'; } while(0)
#endif

using dictionary = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dictionary>;
using used_t = set<string>;

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
            DEBUG("maptel: maptel_delete: map " << id << " deleted");
    }

    void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
        if (tel_src == nullptr || tel_dst == nullptr) {
            DEBUG("maptel: maptel_insert: NULL pointer passed, skipping");
            return;
        }

        string_view tel_src_view { tel_src };
        string_view tel_dst_view { tel_dst };

        if (!tel_is_correct(tel_src_view) || !tel_is_correct(tel_dst_view)) {
            DEBUG("maptel: maptel_insert: telephone number not correct");
            return;
        }

        DEBUG("maptel: maptel_insert(" << id << ", " << tel_src << ", "
                << tel_dst << ")");

        dictionary& dict = get_directory()[id];
        dict.emplace(tel_src_view, tel_dst_view);

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

    static bool transform_helper(dictionary& dict, string tel_src_str, string &res, used_t& used) {
        if (used.count(tel_src_str) != 0) {
            DEBUG("maptel: maptel_transform: cycle detected");
            return false;
        }
        else if (dict.count(tel_src_str) == 0) {
            res = tel_src_str;
            return true;
        }
        else {
            used.insert(tel_src_str);
            return transform_helper(dict, dict.at(tel_src_str), res, used);
        }
    }

    static void update_dst(string tel_src_str, char *tel_dst, size_t len) {
        if (tel_src_str.size() > len) {
            // ERROR (?)
        }
        else {
            for (size_t i = 0; i < tel_src_str.size(); i++) tel_dst[i] = tel_src_str[i];
            tel_dst[tel_src_str.size()] = '\0';
        }
    }

    void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
        dictionary& dict = get_directory()[id];
        string tel_src_str { tel_src };
        DEBUG("maptel: maptel_transform(" << id << ", " << tel_src_str << ", " << reinterpret_cast<void*>(tel_dst) << ", " << len << ")");
        used_t used;
        string res;
        if (!transform_helper(dict, tel_src_str, res, used)) {
            res = tel_src_str;
        }
        update_dst(res, tel_dst, len);
        DEBUG("maptel: maptel_transform: " << tel_src_str << " -> " << res);
    }
}
