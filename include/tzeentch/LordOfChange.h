/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFCHANGE_H
#define LORDOFCHANGE_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class LordOfChange : public Unit
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 380;

    enum WeaponOption
    {
        BalefulSword,
        RodOfSorcery,
        CurvedBeakAndTalons
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    LordOfChange();
    ~LordOfChange() override = default;

    bool configure(WeaponOption option);
    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_rodOfSorcery,
        m_staff,
        m_sword,
        m_beakAndTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Master of Magic                  No
// Spell-eater                      No
// Spell-thief                      No
// Infernal Gateway                 No
// Beacon of Sorcery                No
//

} // namespace Tzeentch

#endif //LORDOFCHANGE_H
