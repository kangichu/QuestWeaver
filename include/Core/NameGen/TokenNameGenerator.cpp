/**
 *
 * @file A fantasy name generator library.
 * @version 1.0.0
 * @license Public Domain
 * @author German M. Bravo (Kronuz)
 *
 */

#include "TokenNameGenerator.h"

#include <cwctype>  // for std::towupper
#include <algorithm>  // for std::reverse

using namespace weave;

const std::unordered_map<std::string, const std::vector<std::string> >
        TokenNameGenerator::symbols = {
        {
                "s", {
                             "ach", "ack", "ad", "age", "ald", "ale", "an", "ang", "ar", "ard",
                             "as", "ash", "at", "ath", "augh", "aw", "ban", "bel", "bur", "cer",
                             "cha", "che", "dan", "dar", "del", "den", "dra", "dyn", "ech", "eld",
                             "elm", "em", "en", "end", "eng", "enth", "er", "ess", "est", "et",
                             "gar", "gha", "hat", "hin", "hon", "ia", "ight", "ild", "im", "ina",
                             "ine", "ing", "ir", "is", "iss", "it", "kal", "kel", "kim", "kin",
                             "ler", "lor", "lye", "mor", "mos", "nal", "ny", "nys", "old", "om",
                             "on", "or", "orm", "os", "ough", "per", "pol", "qua", "que", "rad",
                             "rak", "ran", "ray", "ril", "ris", "rod", "roth", "ryn", "sam",
                             "say", "ser", "shy", "skel", "sul", "tai", "tan", "tas", "ther",
                             "tia", "tin", "ton", "tor", "tur", "um", "und", "unt", "urn", "usk",
                             "ust", "ver", "ves", "vor", "war", "wor", "yer"
                     }
        },
        {
                "v", {
                             "a", "e", "i", "o", "u", "y"
                     }
        },
        {
                "V", {
                             "a", "e", "i", "o", "u", "y", "ae", "ai", "au", "ay", "ea", "ee",
                             "ei", "eu", "ey", "ia", "ie", "oe", "oi", "oo", "ou", "ui"
                     }
        },
        {
                "c", {
                             "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r",
                             "s", "t", "v", "w", "x", "y", "z"
                     }
        },
        {
                "B", {
                             "b", "bl", "br", "c", "ch", "chr", "cl", "cr", "d", "dr", "f", "g",
                             "h", "j", "k", "l", "ll", "m", "n", "p", "ph", "qu", "r", "rh", "s",
                             "sch", "sh", "sl", "sm", "sn", "st", "str", "sw", "t", "th", "thr",
                             "tr", "v", "w", "wh", "y", "z", "zh"
                     }
        },
        {
                "C", {
                             "b", "c", "ch", "ck", "d", "f", "g", "gh", "h", "k", "l", "ld", "ll",
                             "lt", "m", "n", "nd", "nn", "nt", "p", "ph", "q", "r", "rd", "rr",
                             "rt", "s", "sh", "ss", "st", "t", "th", "v", "w", "y", "z"
                     }
        },
        {
                "i", {
                             "air", "ankle", "ball", "beef", "bone", "bum", "bumble", "bump",
                             "cheese", "clod", "clot", "clown", "corn", "dip", "dolt", "doof",
                             "dork", "dumb", "face", "finger", "foot", "fumble", "goof",
                             "grumble", "head", "knock", "knocker", "knuckle", "loaf", "lump",
                             "lunk", "meat", "muck", "munch", "nit", "numb", "pin", "puff",
                             "skull", "snark", "sneeze", "thimble", "twerp", "twit", "wad",
                             "wimp", "wipe"
                     }
        },
        {
                "m", {
                             "baby", "booble", "bunker", "cuddle", "cuddly", "cutie", "doodle",
                             "foofie", "gooble", "honey", "kissie", "lover", "lovey", "moofie",
                             "mooglie", "moopie", "moopsie", "nookum", "poochie", "poof",
                                                                                                                                                                               "poofie", "pookie", "schmoopie", "schnoogle", "schnookie",
                             "schnookum", "smooch", "smoochie", "smoosh", "snoogle", "snoogy",
                             "snookie", "snookum", "snuggy", "sweetie", "woogle", "woogy",
                             "wookie", "wookum", "wuddle", "wuddly", "wuggy", "wunny"
                     }
        },
        {
                "M", {
                             "boo", "bunch", "bunny", "cake", "cakes", "cute", "darling",
                             "dumpling", "dumplings", "face", "foof", "goo", "head", "kin",
                             "kins", "lips", "love", "mush", "pie", "poo", "pooh", "pook", "pums"
                     }
        },
        {
                "D", {
                             "b", "bl", "br", "cl", "d", "f", "fl", "fr", "g", "gh", "gl", "gr",
                             "h", "j", "k", "kl", "m", "n", "p", "th", "w"
                     }
        },
        {
                "d", {
                             "elch", "idiot", "ob", "og", "ok", "olph", "olt", "omph", "ong",
                             "onk", "oo", "oob", "oof", "oog", "ook", "ooz", "org", "ork", "orm",
                             "oron", "ub", "uck", "ug", "ulf", "ult", "um", "umb", "ump", "umph",
                             "un", "unb", "ung", "unk", "unph", "unt", "uzz"
                     }
        }
};


// make_unique is not available in c++11, so we use this template function
// to maintain full c++11 compatibility; std::make_unique is part of C++14.
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


TokenNameGenerator::TokenNameGenerator() {
}


TokenNameGenerator::TokenNameGenerator(std::vector<std::unique_ptr<TokenNameGenerator>> &&generators_) :
        generators(std::move(generators_)) {
}


size_t TokenNameGenerator::combinations() {
    size_t total = 1;
    for (auto &g : generators) {
        total *= g->combinations();
    }
    return total;
}


size_t TokenNameGenerator::min() {
    size_t final = 0;
    for (auto &g : generators) {
        final += g->min();
    }
    return final;
}


size_t TokenNameGenerator::max() {
    size_t final = 0;
    for (auto &g : generators) {
        final += g->max();
    }
    return final;
}

std::string TokenNameGenerator::toString(std::shared_ptr<RandomStream> rs) {
    std::string str;
    for (auto &g : generators) {
        str.append(g->toString(rs));
    }
    return str;
}


void TokenNameGenerator::add(std::unique_ptr<TokenNameGenerator> &&g) {
    generators.push_back(std::move(g));
}


Random::Random() {
}

Random::Random(std::vector<std::unique_ptr<TokenNameGenerator>> &&generators_) :
        TokenNameGenerator(std::move(generators_)) {
}

size_t Random::combinations() {
    size_t total = 0;
    for (auto &g : generators) {
        total += g->combinations();
    }
    return total ? total : 1;
}

size_t Random::min() {
    size_t final = -1;
    for (auto &g : generators) {
        size_t current = g->min();
        if (current < final) {
            final = current;
        }
    }
    return final;
}

size_t Random::max() {
    size_t final = 0;
    for (auto &g : generators) {
        size_t current = g->max();
        if (current > final) {
            final = current;
        }
    }
    return final;
}


std::string Random::toString(std::shared_ptr<RandomStream> rs) {
    if (generators.empty()) {
        return "";
    }
    return generators[rs->GetRandomIndex(generators.size())]->toString(rs);
}


Sequence::Sequence() {
}

Sequence::Sequence(std::vector<std::unique_ptr<TokenNameGenerator>> &&generators_) :
        TokenNameGenerator(std::move(generators_)) {
}

Literal::Literal(const std::string &value_) :
        value(value_) {
}

size_t Literal::combinations() {
    return 1;
}

size_t Literal::min() {
    return value.size();
}

size_t Literal::max() {
    return value.size();
}

std::string Literal::toString(std::shared_ptr<RandomStream>) {
    return value;
}

Reverser::Reverser(std::unique_ptr<TokenNameGenerator> &&g) {
    add(std::move(g));
}


std::string Reverser::toString(std::shared_ptr<RandomStream> rs) {
    std::wstring str = towstring(TokenNameGenerator::toString(rs));
    std::reverse(str.begin(), str.end());
    return tostring(str);
}

Capitalizer::Capitalizer(std::unique_ptr<TokenNameGenerator> &&g) {
    add(std::move(g));
}

std::string Capitalizer::toString(std::shared_ptr<RandomStream> rs) {
    std::wstring str = towstring(TokenNameGenerator::toString(rs));
    str[0] = std::towupper(str[0]);
    return tostring(str);
}


Collapser::Collapser(std::unique_ptr<TokenNameGenerator> &&g) {
    add(std::move(g));
}

std::string Collapser::toString(std::shared_ptr<RandomStream> rs) {
    std::wstring str = towstring(TokenNameGenerator::toString(rs));
    std::wstring out;
    int cnt = 0;
    wchar_t pch = L'\0';
    for (auto ch : str) {
        if (ch == pch) {
            cnt++;
        } else {
            cnt = 0;
        }
        int mch = 2;
        switch (ch) {
            case 'a':
            case 'h':
            case 'i':
            case 'j':
            case 'q':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
                mch = 1;
        }
        if (cnt < mch) {
            out.push_back(ch);
        }
        pch = ch;
    }
    return tostring(out);
}


TokenNameGenerator::TokenNameGenerator(const std::string &pattern, bool collapse_triples) {
    std::unique_ptr<TokenNameGenerator> last;

    std::stack<std::unique_ptr<Group>> stack;
    std::unique_ptr<Group> top = make_unique<GroupSymbol>();

    for (auto c : pattern) {
        switch (c) {
            case '<':
                stack.push(std::move(top));
                top = make_unique<GroupSymbol>();
                break;
            case '(':
                stack.push(std::move(top));
                top = make_unique<GroupLiteral>();
                break;
            case '>':
            case ')':
                if (stack.size() == 0) {
                    throw std::invalid_argument("Unbalanced brackets");
                } else if (c == '>' && top->type != group_types::symbol) {
                    throw std::invalid_argument("Unexpected '>' in pattern");
                } else if (c == ')' && top->type != group_types::literal) {
                    throw std::invalid_argument("Unexpected ')' in pattern");
                }
                last = top->emit();
                top = std::move(stack.top());
                stack.pop();
                top->add(std::move(last));
                break;
            case '|':
                top->split();
                break;
            case '!':
                if (top->type == group_types::symbol) {
                    top->wrap(wrappers::capitalizer);
                } else {
                    top->add(c);
                }
                break;
            case '~':
                if (top->type == group_types::symbol) {
                    top->wrap(wrappers::reverser);
                } else {
                    top->add(c);
                }
                break;
            default:
                top->add(c);
                break;
        }
    }

    if (stack.size() != 0) {
        throw std::invalid_argument("Missing closing bracket");
    }

    std::unique_ptr<TokenNameGenerator> g = top->emit();
    if (collapse_triples) {
        g = make_unique<Collapser>(std::move(g));
    }
    add(std::move(g));
}


TokenNameGenerator::Group::Group(group_types_t type_) :
        type(type_) {
}

void TokenNameGenerator::Group::add(std::unique_ptr<TokenNameGenerator> &&g) {
    while (!wrappers.empty()) {
        switch (wrappers.top()) {
            case reverser:
                g = make_unique<Reverser>(std::move(g));
                break;
            case capitalizer:
                g = make_unique<Capitalizer>(std::move(g));
                break;
        }
        wrappers.pop();
    }
    if (set.size() == 0) {
        set.push_back(make_unique<Sequence>());
    }
    set.back()->add(std::move(g));
}

void TokenNameGenerator::Group::add(char c) {
    std::string value(&c, 1);
    std::unique_ptr<TokenNameGenerator> g = make_unique<Random>();
    g->add(make_unique<Literal>(value));
    Group::add(std::move(g));
}

std::unique_ptr<TokenNameGenerator> TokenNameGenerator::Group::emit() {
    switch (set.size()) {
        case 0:
            return make_unique<Literal>("");
        case 1:
            return std::move(*set.begin());
        default:
            return make_unique<Random>(std::move(set));
    }
}

void TokenNameGenerator::Group::split() {
    if (set.size() == 0) {
        set.push_back(make_unique<Sequence>());
    }
    set.push_back(make_unique<Sequence>());
}

void TokenNameGenerator::Group::wrap(wrappers_t type) {
    wrappers.push(type);
}

TokenNameGenerator::GroupSymbol::GroupSymbol() :
        Group(group_types::symbol) {
}

void TokenNameGenerator::GroupSymbol::add(char c) {
    std::string value(&c, 1);
    std::unique_ptr<TokenNameGenerator> g = make_unique<Random>();
    try {
        for (auto s : TokenNameGenerator::symbols.at(value)) {
            g->add(make_unique<Literal>(s));
        }
    } catch (std::out_of_range) {
        g->add(make_unique<Literal>(value));
    }
    Group::add(std::move(g));
}

TokenNameGenerator::GroupLiteral::GroupLiteral() :
        Group(group_types::literal) {
}

std::wstring towstring(const std::string &s) {
    const char *cs = s.c_str();
    const size_t wn = std::mbsrtowcs(NULL, &cs, 0, NULL);

    if (wn == static_cast<size_t>(-1)) {
        return L"";
    }

    std::vector<wchar_t> buf(wn);
    const size_t wn_again = std::mbsrtowcs(buf.data(), &cs, wn, NULL);

    if (wn_again == static_cast<size_t>(-1)) {
        return L"";
    }

    return std::wstring(buf.data(), wn);
}

std::string tostring(const std::wstring &s) {
    const wchar_t *cs = s.c_str();
    const size_t wn = std::wcsrtombs(NULL, &cs, 0, NULL);

    if (wn == static_cast<size_t>(-1)) {
        return "";
    }

    std::vector<char> buf(wn);
    const size_t wn_again = std::wcsrtombs(buf.data(), &cs, wn, NULL);

    if (wn_again == static_cast<size_t>(-1)) {
        return "";
    }

    return std::string(buf.data(), wn);
}
