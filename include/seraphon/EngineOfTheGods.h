/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ENGINEOFTHEGODS_H
#define ENGINEOFTHEGODS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class EngineOfTheGods : public SeraphonBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    EngineOfTheGods();
    ~EngineOfTheGods() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    void onCharged() override;
    void onStartShooting(PlayerId player) override;

private:

    Weapon m_javelins,
        m_horns,
        m_jaws,
        m_stomps;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   No
// Unstoppable Stampede             Yes
// Steadfast Majesty                No
// Cosmic Engine                    Partial
//

} // namespace Seraphon

#endif //ENGINEOFTHEGODS_H