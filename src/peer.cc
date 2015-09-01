/* peer.cc
   Mathieu Stefani, 12 August 2015
   
*/

#include "peer.h"
#include <iostream>
#include <stdexcept>

namespace Net {

namespace Tcp {

using namespace std;

Peer::Peer()
    : fd_(-1)
{ }

Peer::Peer(const Address& addr)
    : addr(addr)
    , fd_(-1)
{ }

Address
Peer::address() const {
    return addr;
}

string
Peer::hostname() const {
    return hostname_;
}

void
Peer::associateFd(int fd) {
    fd_ = fd;
}

int
Peer::fd() const {
    if (fd_ == -1) {
        throw std::runtime_error("The peer has no associated fd");
    }

    return fd_;
}

void
Peer::putData(std::string name, std::shared_ptr<void> data) {
    auto it = data_.find(name);
    if (it != std::end(data_)) {
        throw std::runtime_error("The data already exists");
    }

    data_.insert(std::make_pair(std::move(name), std::move(data)));
}

std::shared_ptr<void>
Peer::getData(std::string name) const {
    auto data = tryGetData(std::move(name));
    if (data == nullptr) {
        throw std::runtime_error("The data does not exist");
    }

    return data;
}

std::shared_ptr<void>
Peer::tryGetData(std::string(name)) const {
    auto it = data_.find(name);
    if (it == std::end(data_)) return nullptr;

    return it->second;
}

std::ostream& operator<<(std::ostream& os, const Peer& peer) {
    const auto& addr = peer.address();
    os << "(" << addr.host() << ", " << addr.port() << ") [" << peer.hostname() << "]";
    return os;
}

} // namespace Tcp

} // namespace Net