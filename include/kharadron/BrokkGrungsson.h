/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BROKKGRUNGSSON_H
#define BROKKGRUNGSSON_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class BrokkGrungsson : public KharadronBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 240;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BrokkGrungsson();

    ~BrokkGrungsson() override = default;

    bool configure();

protected:

private:

    Weapon m_boast,
        m_charter,
        m_aetherblasters,
        m_saw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Custom-built Dirigible Suit      TODO
// Endrinharness                    TODO
// Hitcher                          TODO
// First Rule of Grungsson          TODO
//

} // namespace KharadronOverlords

#endif //BROKKGRUNGSSON_H
