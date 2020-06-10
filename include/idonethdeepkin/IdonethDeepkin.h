/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IDONETHDEEPKINBASE_H
#define IDONETHDEEPKINBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    enum Enclave {
        Custom,
        Ionrach,
        DhomHain,
        Fuethan,
        Morphann,
        Nautilar,
        Briomdar
    };

    const std::array<int, 7> g_enclave = {
            Custom,
            Ionrach,
            DhomHain,
            Fuethan,
            Morphann,
            Nautilar,
            Briomdar
    };

    class IdonethDeepkinBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        IdonethDeepkinBase() = default;

        ~IdonethDeepkinBase() override = default;

        void setEnclave(Enclave enclave);

    protected:

        IdonethDeepkinBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Enclave m_enclave = Enclave::Custom;
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

    void Init();

} // namespace IdonethDeepkin

#endif // IDONETHDEEPKINBASE_H
