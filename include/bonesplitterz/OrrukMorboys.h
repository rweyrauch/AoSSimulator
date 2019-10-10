/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ORRUKMORBOYS_H
#define ORRUKMORBOYS_H

#include "Unit.h"

namespace Bonesplitterz
{
class SavageOrrukMorboys : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SavageOrrukMorboys();
    ~SavageOrrukMorboys() override = default;

    bool configure(int numModels, bool skullThumper, bool totemBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_chompaAndShiv,
        m_chompaAndShivBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          No
// Totem Bearer                     No
// Spirit of Gorkamorka             No
// Power of the Beast Spirit        No
//

} // namespace Bonesplitterz

#endif //ORRUKMORBOYS_H