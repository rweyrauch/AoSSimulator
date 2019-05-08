/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RUNELORD_H
#define RUNELORD_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Runelord : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Runelord();
    ~Runelord() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    // Runes of Spellbreaking
    int unbindingModifier() const override { return 2; }

private:

    Weapon m_runeStaff,
        m_forgehammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Runes of Spellbreaking           Yes
// Rune Lord
//    Ancestral Shield              No
//    Forgefire                     No
//

} // namespace Dispossessed

#endif //RUNELORD_H