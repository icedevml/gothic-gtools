#include "item.hpp"

std::string oCItem::GetName(int x) {
    ZString str;
    oCItemGetName(this, &str, x);
    return str;
}
