/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONBLASTER_H
#define IRONBLASTER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Ironblaster : public MawtribesBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Ironblaster();
    ~Ironblaster() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartShooting(PlayerId player) override;

private:

    Weapon m_cannonBall,
        m_hailShot,
        m_clubber,
        m_horns,
        m_blade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lethal Payload                   Yes
// Rhinox Charge                    Yes
//

} // namespace OgorMawtribes

#endif //IRONBLASTER_H