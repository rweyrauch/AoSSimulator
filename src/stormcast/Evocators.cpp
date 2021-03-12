/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <stormcast/Evocators.h>
#include <iostream>
#include <stormcast/Empower.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 210;
    static const int g_pointsMaxUnitSize = 840;

    bool Evocators::s_registered = false;

    Evocators::Evocators() :
            StormcastEternal("Evocators", 5, g_wounds, 8, 4, false),
            m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
            m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 5, 3, 3, -1, 1),
            m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
            m_grandStavePrime(Weapon::Type::Melee, "Grandstave", 2, 4, 3, 3, 0, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};
        m_weapons = {&m_tempestBladeAndStave, &m_tempestBladeAndStavePrime, &m_grandStave, &m_grandStavePrime};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool
    Evocators::configure(int numModels, int numGrandstaves, bool primeGrandstave, Lore lore) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        const int maxGrandstaves = numModels;
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
        addModel(primeModel);

        for (auto i = 0; i < numGrandstaves; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_grandStave);
            addModel(model);
        }

        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tempestBladeAndStave);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<Empower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls Evocators::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Celestial Lightning Arc
        if (weapon->isMissile()) {
            return Rerolls::Ones;
        }

        return StormcastEternal::toSaveRerolls(weapon, attacker);
    }

    int Evocators::generateMortalWounds(const Unit *unit) {
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

    Unit *Evocators::Create(const ParameterList &parameters) {
        auto *evos = new Evocators();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool primeGrandstave = GetBoolParam("Prime Grandstave", parameters, false);
        int numGrandstaves = GetIntParam("Grandstaves", parameters, 0);
        auto lore = (Lore) GetEnumParam("Lore of Invigoration", parameters, g_loreOfInvigoration[0]);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        evos->setStormhost(stormhost);

        bool ok = evos->configure(numModels, numGrandstaves, primeGrandstave, lore);
        if (!ok) {
            delete evos;
            evos = nullptr;
        }
        return evos;
    }

    void Evocators::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Prime Grandstave"),
                            IntegerParameter("Grandstaves", 2, 0, g_maxUnitSize, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Lore of Invigoration", g_loreOfInvigoration[0], g_loreOfInvigoration)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Evocators", factoryMethod);
        }
    }

    std::string Evocators::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int Evocators::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int Evocators::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
