/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/CelestarBallista.h>

namespace StormcastEternals
{

Weapon CelestarBallista::s_stormboltsSingle("Celestar Stormbolts: Single Shot", 36, 1, 3, 3, -2, 1);
Weapon CelestarBallista::s_stormboltsRapid("Celestar Stormbolts: Rapid Far", 18, 4, 5, 3, -2, 1);
Weapon CelestarBallista::s_sigmariteBlades("Sigmarite Blades", 1, 4, 4, 4, 0, 1);

CelestarBallista::CelestarBallista() :
    StormcastEternal("Celestar Ballista", 3, WOUNDS, 7, 4, false)
{
    // Burst of Celestial Energy
    s_stormboltsSingle.setHitsPerAttack(RAND_D6);
    s_stormboltsRapid.setHitsPerAttack(RAND_D6);

    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, ORDINATOS, WAR_MACHINE, CELESTAR_BALLISTA};
}

bool CelestarBallista::configure(bool singleShot)
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_sigmariteBlades);
    if (singleShot)
        model.addMissileWeapon(&s_stormboltsSingle);
    else
        model.addMissileWeapon(&s_stormboltsRapid);
    addModel(model);

    return true;
}

} // namespace StormcastEternals
