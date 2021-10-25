#include <string>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include "maptel.h"
using namespace std;

#ifdef NDEBUG
    static constexpr bool debug = false;
#else
    static constexpr bool debug = true;
#endif

#define DEBUG(x)                                         \
    do {                                                 \
        if (debug)                                       \
            std::cerr << "maptel: maptel_" << x << '\n'; \
    } while(0)

#define CHECK_CORRECTNESS(conditional) \
    do {                               \
        if (debug) {                   \
            assert(conditional);       \
        } else if (!(conditional)) {   \
            std::abort();              \
        }                              \
    } while(0)

using dictionary = unordered_map<string, string>;
using directory = unordered_map<unsigned long, dictionary>;
using used_t = set<string>;

namespace jnp1 {
    namespace {
        directory& get_directory() {
            static directory* dir = new directory();
            return *dir;
        }

        void assert_tel_is_correct(const char* tel) {
            CHECK_CORRECTNESS(tel != nullptr);

            for (size_t i = 0; i < jnp1::TEL_NUM_MAX_LEN + 1; ++i) {
                if (tel[i] == '\0')
                    return; /* the end of string */
                else
                    CHECK_CORRECTNESS(isdigit(tel[i]));
            }

            /* Telephone number is larger than 22 characters. */
            CHECK_CORRECTNESS(false);
        }
    }

    dictionary& get_dictionary(unsigned long id) {
        directory& dir = get_directory();
        auto it = dir.find(id);

        assert(it != dir.end());  /* dictionary not found */

        dictionary& dict = it->second;
        return dict;
    }

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
        CHECK_CORRECTNESS(cnt != 0);

        DEBUG("delete: map " << id << " deleted");
    }

    void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
        assert_tel_is_correct(tel_src);
        assert_tel_is_correct(tel_dst);

        DEBUG("insert(" << id << ", " << tel_src << ", " << tel_dst << ")");

        dictionary& dict = get_dictionary(id);
        dict[tel_src] = string(tel_dst);

        DEBUG("insert: inserted");
    }

    void maptel_erase(unsigned long id, char const *tel_src) {
        assert_tel_is_correct(tel_src);

        DEBUG("erase(" << id << ", " << tel_src << ")");

        dictionary& dict = get_dictionary(id);
        size_t cnt = dict.erase(tel_src);

        if (!cnt)
            DEBUG("erase: nothing to erase");
        else
            DEBUG("erase: erased");
    }

    namespace {
        bool transform_helper(dictionary& dict, string& tel_src_str, string &res, used_t& used) {
            if (used.count(tel_src_str) != 0) {
                DEBUG("transform: cycle detected");
                return false;
            }

            auto it = dict.find(tel_src_str);

            if (it == dict.end()) {
                res = tel_src_str;
                return true;
            }
            else {
                used.insert(tel_src_str);
                return transform_helper(dict, it->second, res, used);
            }
        }

        void update_dst(string& tel_src_str, char *tel_dst, size_t len) {
            CHECK_CORRECTNESS(tel_src_str.size() + 1 <= len);

            for (size_t i = 0; i < tel_src_str.size(); i++)
                tel_dst[i] = tel_src_str[i];

            tel_dst[tel_src_str.size()] = '\0';
        }
    }

    void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
        CHECK_CORRECTNESS(tel_dst != nullptr);
        assert_tel_is_correct(tel_src);

        dictionary& dict = get_dictionary(id);

        string tel_src_str { tel_src };
        DEBUG("transform(" << id << ", " << tel_src_str << ", " << "ADDR" << ", " << len << ")");
        used_t used;
        string res;
        if (!transform_helper(dict, tel_src_str, res, used)) {
            res = tel_src_str;
        }
        update_dst(res, tel_dst, len);
        DEBUG("transform: " << tel_src_str << " -> " << res);
    }
}
