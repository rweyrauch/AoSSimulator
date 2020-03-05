/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FROSTLORDSTONEHORN_H
#define FROSTLORDSTONEHORN_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class FrostlordOnStonehorn : public MawtribesBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 400;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    FrostlordOnStonehorn();
    ~FrostlordOnStonehorn() override = default;

    bool configure();
    int move() const override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

private:

    Weapon m_spear,
        m_kicks,
        m_horns,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Earth-shattering Charge          No
// Stone Skeleton                   No
// Bellowing Voice                  No
//

} // namespace OgorMawtribes

#endif //FROSTLORDSTONEHORN_H
