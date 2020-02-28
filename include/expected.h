// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <variant>
#include <exception>
#include <stdexcept>

namespace cat {

template <typename T>
using expected = std::variant<std::exception_ptr, T>;

}
