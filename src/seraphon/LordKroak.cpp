/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/LordKroak.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Roster.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 320;

    bool LordKroak::s_registered = false;

    LordKroak::LordKroak() :
            SeraphonBase("Lord Kroak", 5, WOUNDS, 9, 4, true),
            m_barrier(Weapon::Type::Melee, "Azyrite Force Barrier", 3, 0, 3, 3, -1, 1) {
        m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER, LORD_KROAK};
        m_weapons = {&m_barrier};

        m_totalSpells = 4;
        m_totalUnbinds = 4;
    }

    bool LordKroak::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_barrier);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<CelestialDeliverance>(this));
        //m_knownSpells.push_back(std::make_unique<CometsCall>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Unit *LordKroak::Create(const ParameterList &parameters) {
        auto unit = new LordKroak();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfCelestialDomination[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordKroak::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Lore", g_loreOfCelestialDomination[0], g_loreOfCelestialDomination),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Lord Kroak", factoryMethod);
        }
    }

    int LordKroak::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);

        // Azyrite Force Barrier
        if (target) {
            extra += target->numModelsWithin(attackingModel, 3.0);
        }

        return extra;
    }

    Wounds LordKroak::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = SeraphonBase::applyWoundSave(wounds);

        // Dead for Innumerable Ages
        Dice::RollResult resultNormal, resultMortal;

        Dice::rollD6(wounds.normal, resultNormal);
        Dice::rollD6(wounds.mortal, resultMortal);

        Wounds negatedWounds = {resultNormal.rollsGE(4), resultNormal.rollsGE(4)};
        totalWounds -= negatedWounds;
        return totalWounds.clamp();
    }

    void LordKroak::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Impeccable Foresight
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::rollD6(3, result);
            m_roster->addCommandPoints(result.rollsGE(4));
        }
    }

    int LordKroak::castingModifier() const {
        auto mod = SeraphonBase::castingModifier();
        // Masters of Order
        mod++;
        return mod;
    }

    int LordKroak::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
