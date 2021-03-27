/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class Vokmortian : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Vokmortian();

        ~Vokmortian() override;

    protected:

        void configure(Lore lore);

        int grimWarning(const Unit *unit);

    private:

        Weapon m_gazeOfDeath,
                m_staff;
        Lore m_lore = Lore::Empower_Nadirite_Weapons;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contract of Nagash               TODO
// Grim Warning                     Partial/TODO
// Mortal Touch                     Yes
//

} // namespace OssiarchBonereapers

