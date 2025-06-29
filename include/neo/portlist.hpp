/*
 * Copyright (c) 2025, Joe Inman
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *     https://opensource.org/licenses/MIT
 *
 * This file is part of the Neo Library.
 */

#pragma once

#include <any>
#include <unordered_map>
#include <optional>
#include <string>

namespace jsi::neo
{

class PortList
{
public:
    PortList()  = default;
    ~PortList() = default;

    template <typename T>
    inline void set(const std::string& key, const T& value)
    {
        data_[key] = value;
    }

    template <typename T>
    inline std::optional<T> get(const std::string& key)
    {
        auto it = data_.find(key);
        if (it != data_.end())
        {
            if (it->second.type() != typeid(T))
            {
                return std::nullopt;
            }

            return std::any_cast<T>(it->second);
        }
        return std::nullopt;
    }

private:
    static inline std::unordered_map<std::string, std::any> data_;
};

}  // namespace jsi::neo
