/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHMAGEONDRAGON_H
#define ARCHMAGEONDRAGON_H

#include <Unit.h>
#include <Weapon.h>

namespace EldritchCouncil
{

class ArchmageOnDragon : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ArchmageOnDragon();
    ~ArchmageOnDragon() override = default;

    bool configure(bool arcaneTome, bool talisman);

    int move() const override;

protected:

    void onWounded() override;
    int unbindingModifier() const override;

    int getDamageTableIndex() const;

private:

    bool m_arcaneTome = false;
    bool m_talismanOfArcanePower = false;

    Weapon m_magestaff,
        m_sorcerousBlade,
        m_dragonClaws,
        m_dragonJaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Talisman of Arcane Power         Yes
// Dragonfire                       No
// Drain Magic                      No
//

} // namespace EldritchCouncil

#endif //ARCHMAGEONDRAGON_H