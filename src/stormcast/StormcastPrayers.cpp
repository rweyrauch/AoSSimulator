/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/StormcastPrayers.h>
#include <Unit.h>

namespace StormcastEternals {
    class Sanction : public DamagePrayer {
    public:
        explicit Sanction(Unit *priest) :
                DamagePrayer(priest, "Sanction", 4, 7.0, RAND_D3) {}

    protected:
        int getDamage(Unit *target, int prayingRoll) const override;
    };

    int Sanction::getDamage(Unit *target, int prayingRoll) const {
        if (target->hasKeyword(WIZARD))
            return m_damage;
        return 0;
    }

    Prayer *CreateSanction(Unit *priest) {
        return new Sanction(priest);
    }

    Prayer *CreateHealingStorm(Unit *priest) {
        return new HealPrayer(priest, "Healing Storm", 3, 12.0, RAND_D3);
    }

    class LightningStorm : public DamagePrayer {
    public:
        explicit LightningStorm(Unit *priest) :
                DamagePrayer(priest, "Lightning Storm", 3, 12.0, RAND_D3) {}

    protected:

        void secondaryEffect(Unit *target, int round) const override;
    };

    void LightningStorm::secondaryEffect(Unit *target, int round) const {
        if (target) {
            target->buffModifier(To_Hit_Melee, -1, {Phase::Hero, round + 1, m_priest->owningPlayer()});
            target->buffModifier(To_Hit_Missile, -1, {Phase::Hero, round + 1, m_priest->owningPlayer()});
        }
    }

    Prayer *CreateLightningStorm(Unit *priest) {
        return new LightningStorm(priest);
    }

} // namespace StormcastEternals
