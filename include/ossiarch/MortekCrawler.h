/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORTEKCRAWLER_H
#define MORTEKCRAWLER_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class MortekCrawler : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    MortekCrawler();
    ~MortekCrawler() override = default;

    bool configure();

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_catapultSkulls,
        m_catapultCauldron,
        m_catapultStele,
        m_tools;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dread Catapult                   No
// Cauldron of Torment              No
// Cursed Stele                     No
//

} // namespace OssiarchBonereapers

#endif //MORTEKCRAWLER_H