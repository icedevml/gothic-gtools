#include "gothic.hpp"
#include "multi.hpp"
#include "main.hpp"

#include <sstream>
#include <string>

void MultiEngine::onBodystateChange(int subtype, VEC pos) {
    std::stringstream tmp;
    tmp << "D: ME::onBodystateChange(" << subtype << ")" << std::endl;
    tmp << "D: -> " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    Error::Warning(Error::instance, tmp.str().c_str());
}

void MultiEngine::onItemuseMessage(int subtype, VEC pos) {
    std::stringstream tmp;
    tmp << "D: ME::onItemuseMessage(" << subtype << ")" << std::endl;
    tmp << "D: -> " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    Error::Warning(Error::instance, tmp.str().c_str());
}
