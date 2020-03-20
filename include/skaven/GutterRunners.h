/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GUTTERRUNNERS_H
#define GUTTERRUNNERS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class GutterRunners : public Skaventide
{
public:
    static const int POINTS_MAX_UNIT_SIZE = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GutterRunners();
    ~GutterRunners() override = default;

    bool configure(int numModels);

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_throwingStars,
        m_punchDaggerAndBlade;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Running Death                    Yes
// Sneaky Infiltrators              TODO
// Throwing Stars                   Yes

} // namespace Skaven

#endif //GUTTERRUNNERS_H