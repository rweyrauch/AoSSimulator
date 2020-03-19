/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FROSTSABRES_H
#define FROSTSABRES_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class FrostSabres : public MawtribesBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 40;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    FrostSabres();
    ~FrostSabres() override = default;

    bool configure(int numModels);

protected:

    int chargeModifier() const override;
    int braveryModifier() const override;

private:

    Weapon m_fangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Their Master's Voice             Yes
//

} // namespace OgorMawtribes

#endif //FROSTSABRES_H
