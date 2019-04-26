/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/StormcastPrayers.h>

namespace StormcastEternals
{

Prayer *CreateSanction(Unit* priest)
{
    return nullptr;
}

Prayer *CreateHealingStorm(Unit *priest)
{
    return new HealPrayer(priest, "Healing Storm", 3, 12.0f, RAND_D3);
}

Prayer *CreateLightningStorm(Unit *priest)
{
    // TODO: Add -1 to hit debuff
    return new DamagePrayer(priest, "Lightning Storm", 3, 12.0f, RAND_D3);
}

} // namespace StormcastEternals
