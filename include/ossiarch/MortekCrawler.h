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

    class MortekCrawler : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit MortekCrawler(Legion legion);

        MortekCrawler() = delete;

        ~MortekCrawler() override = default;

    protected:

        void onRestore() override;

        void onWounded() override;

    private:

        size_t getDamageTableIndex() const;

        bool m_usedCauldronOfTorment = false;
        bool m_usedCursedSteel = false;

        Weapon  m_catapultSkulls{Weapon::Type::Missile, "Dread Catapult - Necrotic Skulls", 36, 3, 2, 3, 0, 5},
                m_catapultCauldron{Weapon::Type::Missile, "Dread Catapult - Cauldron of Torment", 36, 1, 0, 0, 0, 0},
                m_catapultStele{Weapon::Type::Missile, "Dread Catapult - Cursed Stele", 36, 1, 0, 0, 0, 0},
                m_tools{Weapon::Type::Melee, "Crawler Tools", 1, 6, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dread Catapult                   TODO
// Cauldron of Torment              TODO
// Cursed Stele                     TODO
//

} // namespace OssiarchBonereapers

