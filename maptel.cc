#include <string>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cctype>
#include <cassert>
#include "maptel.h"
using namespace std;

#ifdef NDEBUG
    #define DEBUG(x) do { } while(0)
#else
    #define DEBUG(x) do { cerr << "maptel: maptel_" << x << '\n'; } while(0)
#endif

using dictionary = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dictionary>;
using used_t = set<string>;

static directory& get_directory() {
    static directory* dir = new directory();
    return *dir;
}

static void assert_tel_is_correct(const char* tel) {
#ifndef NDEBUG
    assert(tel != nullptr);

    for (size_t i = 0; i < jnp1::TEL_NUM_MAX_LEN + 1; ++i) {
        if (tel[i] == '\0')
            return; /* the end of string */
        else
            assert(isdigit(tel[i]));
    }

    /* Telephone number is larger than 22 characters. */
    assert(false);
#endif
}

static dictionary* check_get_dictionary(unsigned long id) {
    directory& dir = get_directory();
    auto it = dir.find(id);

    if (it == dir.end())
        assert(false); /* dictionary not found */
    else {
        dictionary& dict = it->second;
        return &dict;
    }
}

namespace jnp1 {
    unsigned long maptel_create(void) {
        DEBUG("create()");

        directory& dir = get_directory();
        static unsigned long gid = 0;
        dir.emplace(gid, dictionary());

        DEBUG("create: new map id = " << gid);
        return gid++;
    }

    void maptel_delete(unsigned long id) {
        DEBUG("delete(" << id << ")");

        directory& dir = get_directory();
        size_t cnt = dir.erase(id);
        assert(cnt != 0);

        DEBUG("delete: map " << id << " deleted");
    }

    void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
        assert_tel_is_correct(tel_src);
        assert_tel_is_correct(tel_dst);

        DEBUG("insert(" << id << ", " << tel_src << ", "
                << tel_dst << ")");

        dictionary* dict = check_get_dictionary(id);

        (*dict)[tel_src] = string(tel_dst);

        DEBUG("insert: inserted");
    }

    void maptel_erase(unsigned long id, char const *tel_src) {
        assert_tel_is_correct(tel_src);

        DEBUG("erase(" << id << ", " << tel_src << ")");

        dictionary* dict = check_get_dictionary(id);
        assert(dict != nullptr);

        size_t cnt = dict->erase(tel_src);

        if (!cnt)
            DEBUG("erase: nothing to erase");
        else
            DEBUG("erase: erased");
    }

    static bool transform_helper(dictionary& dict, string& tel_src_str, string &res, used_t& used) {
        if (used.count(tel_src_str) != 0) {
            DEBUG("transform: cycle detected");
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

    static void update_dst(string& tel_src_str, char *tel_dst, size_t len) {
        assert(tel_src_str.size() + 1 <= len);

        for (size_t i = 0; i < tel_src_str.size(); i++)
            tel_dst[i] = tel_src_str[i];

        tel_dst[tel_src_str.size()] = '\0';
    }

    void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
        assert(tel_dst != nullptr);
        assert_tel_is_correct(tel_src);

        dictionary* dict = check_get_dictionary(id);
        assert(dict != nullptr);

        string tel_src_str { tel_src };
        DEBUG("transform(" << id << ", " << tel_src_str << ", " << "ADDR" << ", " << len << ")");
        used_t used;
        string res;
        if (!transform_helper(*dict, tel_src_str, res, used)) {
            res = tel_src_str;
        }
        update_dst(res, tel_dst, len);
        DEBUG("transform: " << tel_src_str << " -> " << res);
    }
}
