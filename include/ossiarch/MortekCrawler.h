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

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        MortekCrawler();

        ~MortekCrawler() override = default;

        bool configure();

    protected:

        void onWounded() override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_catapultSkulls,
                m_catapultCauldron,
                m_catapultStele,
                m_tools;

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

