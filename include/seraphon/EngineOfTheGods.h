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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    EngineOfTheGods();
    ~EngineOfTheGods() override;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    void onCharged() override;
    void onStartShooting(PlayerId player) override;

    Rerolls steadfastMajestyBraveryReroll(const Unit* unit);

private:

    Weapon m_javelins,
        m_horns,
        m_jaws,
        m_stomps;

    lsignal::slot m_steadfastSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   TODO
// Unstoppable Stampede             Yes
// Steadfast Majesty                Yes
// Cosmic Engine                    Partial/TODO
//

} // namespace Seraphon

#endif //ENGINEOFTHEGODS_H