/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/TheMasque.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool TheMasque::s_registered = false;

    TheMasque::TheMasque() :
            SlaaneshBase("The Masque", 10, g_wounds, 10, 5, false),
            m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 6, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, HERALD_OF_SLAANESH, THE_MASQUE};
        m_weapons = {&m_ravagingClaws};
        m_battleFieldRole = Role::Leader;

        // Lithe and Swift
        m_runAndCharge = true;

        // The Endless Dance
        m_pileInMove = 6;
    }

    void TheMasque::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_ravagingClaws);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *TheMasque::Create(const ParameterList &parameters) {
        auto unit = new TheMasque();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void TheMasque::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheMasque::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    TheMasque::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("The Masque", factoryMethod);
        }
    }

    int TheMasque::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void TheMasque::onStartHero(PlayerId player) {
        SlaaneshBase::onStartHero(player);

        // Staff of Masks
        if (owningPlayer() == player) {
            if (remainingWounds() < wounds()) {
                heal(Dice::RollD3());
            } else {
                buffModifier(Attribute::Attacks_Melee, Dice::RollD3(),
                             {Phase::Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

    Rerolls TheMasque::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // The Endless Dance
        if (target->move() <= 5) return Rerolls::Failed;
        return SlaaneshBase::toWoundRerolls(weapon, target);
    }

    Rerolls TheMasque::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // The Endless Dance
        if (target->move() <= 10) return Rerolls::Failed;
        return SlaaneshBase::toHitRerolls(weapon, target);
    }

    Wounds TheMasque::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Inhuman Reflexes
        Wounds totalWounds = ignoreWounds(wounds, 4);
        return SlaaneshBase::applyWoundSave(totalWounds, attackingUnit);
    }

} // Slannesh
