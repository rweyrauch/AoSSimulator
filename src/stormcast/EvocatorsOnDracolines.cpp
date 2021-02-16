/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <stormcast/EvocatorsOnDracolines.h>
#include <iostream>
#include <stormcast/Empower.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 260;
    static const int g_pointsMaxUnitSize = 260 * 4;

    bool EvocatorsOnCelestialDracolines::s_registered = false;

    EvocatorsOnCelestialDracolines::EvocatorsOnCelestialDracolines() :
            StormcastEternal("Evocators on Celestial Dracolines", 12, g_wounds, 8, 4, false),
            m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
            m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 5, 3, 3, -1, 1),
            m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
            m_grandStavePrime(Weapon::Type::Melee, "Grandstave", 2, 4, 3, 3, 0, 2),
            m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};
        m_weapons = {&m_tempestBladeAndStave, &m_tempestBladeAndStavePrime, &m_grandStave, &m_grandStavePrime,
                     &m_monstrousClaws};
        m_hasMount = true;

        s_globalBraveryMod.connect(this, &EvocatorsOnCelestialDracolines::supernaturalRoar, &m_connection);

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    EvocatorsOnCelestialDracolines::~EvocatorsOnCelestialDracolines() {
        m_connection.disconnect();
    }

    bool EvocatorsOnCelestialDracolines::configure(int numModels, int numGrandstaves, bool primeGrandstave,
                                                   Lore lore) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        const int maxGrandstaves = numModels / g_minUnitSize * 2;
        if (numGrandstaves > maxGrandstaves) {
            // Invalid weapon configuration.
            return false;
        }

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (primeGrandstave) {
            primeModel->addMeleeWeapon(&m_grandStavePrime);
            numGrandstaves--;
        } else {
            primeModel->addMeleeWeapon(&m_tempestBladeAndStavePrime);
        }
        primeModel->addMeleeWeapon(&m_monstrousClaws);
        addModel(primeModel);

        for (auto i = 0; i < numGrandstaves; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_grandStave);
            model->addMeleeWeapon(&m_monstrousClaws);
            addModel(model);
        }

        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tempestBladeAndStave);
            model->addMeleeWeapon(&m_monstrousClaws);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<Empower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls EvocatorsOnCelestialDracolines::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Celestial Lightning Arc
        if (weapon->isMissile()) {
            return Reroll_Ones;
        }

        return StormcastEternal::toSaveRerolls(weapon, attacker);
    }

    int EvocatorsOnCelestialDracolines::generateMortalWounds(const Unit *unit) {
        auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

        // Celestial Lightning Arc
        int roll = Dice::RollD6();
        if (roll >= 4) {
            mortalWounds++;
        }
        roll = Dice::RollD6();
        if (roll >= 4) {
            mortalWounds++;
        }

        return mortalWounds;
    }

    Unit *EvocatorsOnCelestialDracolines::Create(const ParameterList &parameters) {
        auto *evos = new EvocatorsOnCelestialDracolines();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool primeGrandstave = GetBoolParam("Prime Grandstave", parameters, false);
        int numGrandstaves = GetIntParam("Grandstaves", parameters, 0);
        auto invigoration = (Lore) GetEnumParam("Lore of Invigoration", parameters, g_loreOfInvigoration[0]);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        evos->setStormhost(stormhost);

        bool ok = evos->configure(numModels, numGrandstaves, primeGrandstave, invigoration);
        if (!ok) {
            delete evos;
            evos = nullptr;
        }
        return evos;
    }

    void EvocatorsOnCelestialDracolines::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Prime Grandstave"),
                            IntegerParameter("Grandstaves", 2, 0, g_maxUnitSize / g_minUnitSize * 2, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Lore of Invigoration", g_loreOfInvigoration[0], g_loreOfInvigoration)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Evocators on Celestial Dracolines", factoryMethod);
        }
    }

    Rerolls EvocatorsOnCelestialDracolines::chargeRerolls() const {
        if (m_charged)
            return Reroll_Failed;
        return StormcastEternal::chargeRerolls();
    }

    std::string EvocatorsOnCelestialDracolines::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int EvocatorsOnCelestialDracolines::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int EvocatorsOnCelestialDracolines::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int EvocatorsOnCelestialDracolines::supernaturalRoar(const Unit *target) {
        // Supernatural Roar
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

} // namespace StormcastEternals
