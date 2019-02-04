/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{
class Zombies : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 60;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Zombies();
    ~Zombies() override = default;

    bool configure(int numModels, bool standardBearer, bool noiseMaker);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* target) const override;
    int toWoundModifier(const Weapon* weapon, const Unit* target) const override;

private:

    bool m_standardBearer;
    bool m_noiseMaker;

    Weapon m_zombieBite;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dragged Down and Torn Apart      Yes
// The Newly Dead                   No
// Vigour Mortis                    Yes
//

} //namespace Death

#endif //ZOMBIES_H