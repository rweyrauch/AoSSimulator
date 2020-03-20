/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{
class Zombies : public LegionOfNagashBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Zombies();
    ~Zombies() override = default;

    bool configure(int numModels, bool standardBearer, bool noiseMaker);

protected:

    int toHitModifier(const Weapon* weapon, const Unit* target) const override;
    int toWoundModifier(const Weapon* weapon, const Unit* target) const override;

private:

    bool m_standardBearer = false;
    bool m_noiseMaker = false;

    Weapon m_zombieBite;

    static bool s_registered;

};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  TODO
// Noise Maker                      TODO
// Dragged Down and Torn Apart      Yes
// The Newly Dead                   TODO
// Vigour Mortis                    Yes
//

} //namespace Death

#endif //ZOMBIES_H