/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FELLWATERTROGGOTHS_H
#define FELLWATERTROGGOTHS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class FellwaterTroggoths : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    FellwaterTroggoths();
    ~FellwaterTroggoths() override = default;

    bool configure(int numModels);

protected:

    void onStartHero(PlayerId player) override;
    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_noxiousVomit,
        m_spikedClub;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Regeneration                     Yes
// Terrible Stench                  Yes
//

} // namespace GloomspiteGitz

#endif //FELLWATERTROGGOTHS_H