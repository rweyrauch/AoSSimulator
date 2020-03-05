/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HUSKARDSTONEHORN_H
#define HUSKARDSTONEHORN_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class HuskardOnStonehorn : public MawtribesBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    HuskardOnStonehorn();
    ~HuskardOnStonehorn() override = default;

    bool configure();
    int move() const override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

private:

    Weapon m_harpoon,
        m_chaintrap,
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
// Blood Vulture                    No
// Line-breakers                    No
//

} // namespace OgorMawtribes

#endif //HUSKARDSTONEHORN_H
