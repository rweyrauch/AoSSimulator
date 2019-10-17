/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BOINGROTBOUNDERZ_H
#define BOINGROTBOUNDERZ_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class BoingrotBounderz : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    BoingrotBounderz();
    ~BoingrotBounderz() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onCharged() override;
    int toWoundModifier(const Weapon* weapon, const Unit* unit) const override;

private:

    Weapon m_fangFilledGob,
        m_pokinLance,
        m_pokinLanceBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Boing! Smash!                    Yes
// Lances of the Bounderz           Yes
//

} // namespace GloomspiteGitz

#endif //BOINGROTBOUNDERZ_H