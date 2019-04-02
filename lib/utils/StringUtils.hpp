// Copyright (c) Microsoft. All rights reserved.
#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include "Version.hpp"
#include "ctmacros.hpp"

#include <string>
#include <vector>

namespace ARIASDK_NS_BEGIN
{
    class StringUtils
    {
    public:

        static void SplitString(const std::string& s, const char separator, std::vector<std::string>& parts);
        static bool AreAllCharactersWhitelisted(const std::string& stringToTest, const std::string& whitelist);

    private:

        StringUtils(const StringUtils&) = delete;
        StringUtils& operator=(const StringUtils&) = delete;
    };

} ARIASDK_NS_END
#endif
