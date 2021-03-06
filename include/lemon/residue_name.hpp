#ifndef RESIDUE_NAME_HPP
#define RESIDUE_NAME_HPP

#include <array>
#include <string>
#include <cstring>
#include <unordered_map>
#include <set>
#include <ostream>

namespace lemon
{

class ResidueName : private std::array<char, 3>
{
    using super = std::array<char, 3>;
    friend bool operator==(const ResidueName &lhs, const ResidueName &rhs);
    friend bool operator==(const ResidueName &lhs, const std::string &rhs);
    friend bool operator==(const std::string &lhs, const ResidueName &rhs);
    friend bool operator!=(const ResidueName &lhs, const ResidueName &rhs);
    friend bool operator!=(const ResidueName &lhs, const std::string &rhs);
    friend bool operator!=(const std::string &lhs, const ResidueName &rhs);
    friend bool operator<(const ResidueName &lhs, const ResidueName &rhs);
    friend std::ostream &operator<<(std::ostream &os, const ResidueName &);
    using super::operator[];

    char check_digit_(char c) const
    {
#ifndef NDEBUG
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')))
        {
            throw std::range_error("Invalid character");
        }
#endif
        return c;
    }

    char clamp_(char c) const
    {
        if (c == 0)
        {
            return 36;
        }

        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }

        return 10 + c - 'A';
    }

  public:
    ResidueName(const std::string& s) : super({{0, 0, 0}})
    {
        switch (s.length())
        {
        case 3:
            (*this)[2] = check_digit_(s[2]);
        case 2:
            (*this)[1] = check_digit_(s[1]);
        case 1:
            (*this)[0] = check_digit_(s[0]);
            break;
        default:
            throw std::length_error("Cannot have a residue name with given character length");
            break;
        }
    }
    ResidueName(const char* s) : super({{0, 0, 0}})
    {
        switch (std::strlen(s))
        {
        case 3:
            (*this)[2] = check_digit_(s[2]);
        case 2:
            (*this)[1] = check_digit_(s[1]);
        case 1:
            (*this)[0] = check_digit_(s[0]);
            break;
        default:
            throw std::length_error("Cannot have a residue name with given character length");
            break;
        }
    }

    unsigned short hash() const {
        return clamp_((*this)[0]) + clamp_((*this)[1]) * 37 + clamp_((*this)[2]) * (37 * 37);
    }
    const std::array<char, 3> &operator*() const {
        return *this;
    }
};

struct ResidueNameHash
{
    unsigned short operator()(const ResidueName &resn) const {
        return resn.hash();
    }
};

inline bool operator==(const ResidueName& lhs, const ResidueName& rhs)
{
    return (lhs[0] == rhs[0]) &&
           (lhs[1] == rhs[1]) &&
           (lhs[2] == rhs[2]);
}

inline bool operator==(const ResidueName& lhs, const std::string& rhs)
{
    switch (rhs.length())
    {
    case 3:
        return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) && (lhs[2] == rhs[2]);
        break;
    case 2:
        return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) && (lhs[2] == 0);
        break;
    case 1:
        return (lhs[0] == rhs[0]) && (lhs[1] == 0) && (lhs[2] == 0);
        break;
    }

    return false;
}

inline bool operator==(const std::string& lhs, const ResidueName& rhs)
{
    return rhs == lhs;
}

inline bool operator!=(const ResidueName& lhs, const ResidueName& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const ResidueName& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const std::string& lhs, const ResidueName& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const ResidueName& lhs, const ResidueName& rhs)
{
    if (lhs[0] < rhs[0])
    {
        return true;
    }

    if (lhs[0] > rhs[0])
    {
        return false;
    }

    if (lhs[1] < rhs[1])
    {
        return true;
    }

    if (lhs[1] > rhs[1])
    {
        return false;
    }

    if (lhs[2] < rhs[2])
    {
        return true;
    }

    return false;
}

typedef std::unordered_map<ResidueName, std::size_t, ResidueNameHash> ResidueNameCount;
typedef std::set<ResidueName> ResidueNameSet;

inline std::ostream& operator<<(std::ostream& os, const ResidueName& res_name) {
    auto& resn = *res_name;
    os <<
        (resn[0]? resn[0] : ' ') <<
        (resn[1]? resn[1] : ' ') <<
        (resn[2]? resn[2] : ' ')
    ;
    return os;
}

inline ResidueNameCount& operator+=(ResidueNameCount& lhs, const ResidueNameCount& rhs) {
    for (auto iter : rhs) {
        lhs[iter.first] += iter.second;
    }
    return lhs;
}

inline std::ostream& operator<<(std::ostream& os, const ResidueNameCount& rnc) {
    for (auto i : rnc) {
        os << "\t" << i.first << "\t" << i.second;
    }
    return os;
}
}

#endif
