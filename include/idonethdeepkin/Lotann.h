/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOTANN_H
#define LOTANN_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class Lotann : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Lotann();
    ~Lotann() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;
    int catalogueOfSouls(const Unit* target);

private:

    Weapon m_quill,
        m_cudgel,
        m_blade,
        m_tentacles;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Catalogue of Souls               Yes
// Writhing Tentacles               Yes
//

} // namespace IdonethDeepkin

#endif // LOTANN_H
