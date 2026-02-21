#include "core/domain/user.h"

User::User(const std::string& username) : id_(-1), username_(username) {}

User::User(int id, const std::string& username, const std::string& display_name)
    : id_(id), username_(username), display_name_(display_name) {}