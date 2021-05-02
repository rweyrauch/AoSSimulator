/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/TheDreadPageant.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 120;

    bool TheDreadPageant::s_registered = false;

    TheDreadPageant::TheDreadPageant() :
            SlaaneshBase("The Dread Pageant", 6, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_bow(Weapon::Type::Missile, "Blissbarb Bow", 18, 2, 4, 4, -1, 1),
            m_clawAndWeapon(Weapon::Type::Melee, "Crushing Claw and Gilded Weapon", 1, 4, 4, 3, -2, 2),
            m_glaive(Weapon::Type::Melee, "Slickblade Glaive", 1, 3, 3, 3, -1, 1),
            m_harpoon(Weapon::Type::Melee, "Dread Harpoon", 1, 4, 2, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, DREAD_PAGEANT};
        m_weapons = {&m_bow, &m_clawAndWeapon, &m_glaive, &m_harpoon};
    }

    void TheDreadPageant::configure() {
        auto vasillac = new Model(g_basesize, wounds() + 2);
        vasillac->setName("Vasillac");
        vasillac->addMeleeWeapon(&m_harpoon);
        addModel(vasillac);

        auto slakeslash = new Model(g_basesize, wounds() + 2);
        slakeslash->setName("Slakeslash");
        slakeslash->addMeleeWeapon(&m_clawAndWeapon);
        addModel(slakeslash);

        auto glissete = new Model(g_basesize, wounds());
        glissete->setName("Glissete");
        glissete->addMeleeWeapon(&m_glaive);
        addModel(glissete);

        auto hadzu = new Model(g_basesize, wounds());
        hadzu->setName("Hadzu");
        hadzu->addMissileWeapon(&m_bow);
        addModel(hadzu);
    }

    Unit *TheDreadPageant::Create(const ParameterList &parameters) {
        auto unit = new TheDreadPageant();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        unit->configure();
        return unit;
    }

    void TheDreadPageant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheDreadPageant::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    TheDreadPageant::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host)
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("The Dread Pageant", factoryMethod);
        }
    }

    int TheDreadPageant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds TheDreadPageant::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        Wounds totalWounds = wounds;

        if (isNamedModelAlive("Vasillac")) {
            // Art of the Myrmidesh
            Dice::RollResult woundSaves, mortalSaves;
            Dice::RollD6(wounds.normal, woundSaves);
            Dice::RollD6(wounds.mortal, mortalSaves);

            totalWounds.normal -= woundSaves.rollsGE(4);
            totalWounds.normal = std::max(totalWounds.normal, 0);
            totalWounds.mortal -= mortalSaves.rollsGE(4);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);
        }

        return totalWounds;
    }

    Wounds TheDreadPageant::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadliest Procession
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return SlaaneshBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // Slannesh
