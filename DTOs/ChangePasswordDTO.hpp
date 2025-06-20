#pragma once
#include <iostream>

struct ChangePasswordDTO
{
    int Entry;
    std::string Code;
    std::string OldPassword;
    std::string NewPassword;
};
