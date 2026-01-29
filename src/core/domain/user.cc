#include "core/domain/user.h"

User::User(const std::string& name) : id_(-1), name_(name) {}

User::User(int id, const std::string& name) : id_(id), name_(name) {}