/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/MorathiHighOracle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool MorathiHighOracleOfKhaine::s_registered = false;

    MorathiHighOracleOfKhaine::MorathiHighOracleOfKhaine() :
            DaughterOfKhaine("Morathi, Khaine", 6, g_wounds, 8, 4, false),
            m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 3, 3, 3, -1, RAND_D3),
            m_wings(Weapon::Type::Melee, "Bladed Wings", 2, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, WIZARD, MORATHI_KHAINE};
        m_weapons = {&m_heartrender, &m_wings};
        m_battleFieldRole = Leader;

        m_totalSpells = 3;
        m_totalUnbinds = 2;
    }

    bool MorathiHighOracleOfKhaine::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_heartrender);
        model->addMeleeWeapon(&m_wings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *MorathiHighOracleOfKhaine::Create(const ParameterList &parameters) {
        auto unit = new MorathiHighOracleOfKhaine();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MorathiHighOracleOfKhaine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Morathi, Khaine", factoryMethod);
        }
    }

    int MorathiHighOracleOfKhaine::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = DaughterOfKhaine::targetHitModifier(weapon, attacker);

        // Commanding Presence
        mod--;

        return mod;
    }

    int MorathiHighOracleOfKhaine::castingModifier() const {
        auto mod = Unit::castingModifier();
        // Sorceress Supreme
        mod++;
        return mod;
    }

    int MorathiHighOracleOfKhaine::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        // Sorceress Supreme
        mod++;
        return mod;
    }

    Wounds MorathiHighOracleOfKhaine::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = DaughterOfKhaine::applyWoundSave(wounds);

        return totalWounds;
    }

    int MorathiHighOracleOfKhaine::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace DaughtersOfKhaine

