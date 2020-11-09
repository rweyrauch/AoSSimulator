/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AOSSIMPRIVATE_H
#define AOSSIMPRIVATE_H

#include <magic_enum.hpp>

template <typename E>
constexpr auto ToInteger(magic_enum::Enum<E> value) {
    // magic_enum::Enum<E> - C++17 Concept for enum type.
    return static_cast<magic_enum::underlying_type_t<E>>(value);
}

#endif // AOSSIMPRIVATE_H