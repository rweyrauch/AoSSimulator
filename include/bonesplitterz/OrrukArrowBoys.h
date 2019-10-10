/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ORRUKARROWBOYS_H
#define ORRUKARROWBOYS_H

#include "Unit.h"

namespace Bonesplitterz
{

class SavageOrrukArrowboys : public Unit
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

    SavageOrrukArrowboys();
    ~SavageOrrukArrowboys() override = default;

    bool configure(int numModels, bool skullThumper, bool totemBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_stingaBow,
        m_boneShiv,
        m_chompa;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          No
// Totem Bearer                     No
// Aim Fer Its Eyes                 No
// Loadsa Arrows                    No
//

} // namespace Bonesplitterz

#endif //ORRUKARROWBOYS_H