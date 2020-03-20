/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NIGHTHAUNT_H
#define NIGHTHAUNT_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt {

    class Nighthaunt : public Unit {
    public:
        Nighthaunt() = default;

        ~Nighthaunt() override = default;

    protected:
        Nighthaunt(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Wounds applyWoundSave(const Wounds &wounds) override;

        int toSaveModifier(const Weapon *weapon) const override;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Dread                    TODO
// Deathless Spirits                Yes
// From the Underworlds They Come   TODO
// Feed on Terror                   TODO
// Wave of Terror                   TODO
// Spectral Summons                 TODO
//

    void Init();

} // namespace Nighthaunt

#endif // NIGHTHAUNT_H