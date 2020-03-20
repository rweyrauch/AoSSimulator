/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/VandusHammerhand.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 280;

    bool VandusHammerhand::s_registered = false;

    VandusHammerhand::VandusHammerhand() :
            StormcastEternal("Vandus Hammerhand", 10, WOUNDS, 9, 3, false),
            m_heldensen(Weapon::Type::Melee, "Heldensen", 2, 3, 3, 2, -1, 3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, HAMMERS_OF_SIGMAR, LORD_CELESTANT,
                      VANDUS_HAMMERHAND};
        m_weapons = {&m_heldensen, &m_clawsAndFangs};

        s_globalBraveryMod.connect(this, &VandusHammerhand::lordOfTheHammerhandsBraveryMod, &m_lordSlot);
    }

    VandusHammerhand::~VandusHammerhand() {
        m_lordSlot.disconnect();
    }

    bool VandusHammerhand::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_heldensen);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *VandusHammerhand::Create(const ParameterList &parameters) {
        auto unit = new VandusHammerhand();

        unit->setStormhost(HammersOfSigmar);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VandusHammerhand::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vandus Hammerhand", factoryMethod);
        }
    }

    int VandusHammerhand::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);

        // Heldensen
        if (m_charged && weapon->name() == m_heldensen.name()) {
            attacks += Dice::rollD3();
        }
        return attacks;
    }

    Wounds VandusHammerhand::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            // D6 instead of 1
            return {Dice::rollD6(), 0};
        }
        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int VandusHammerhand::lordOfTheHammerhandsBraveryMod(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(HAMMERS_OF_SIGMAR) && (distanceTo(unit) <= 24.0f)) return 42;
        return 0;
    }

    int VandusHammerhand::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals