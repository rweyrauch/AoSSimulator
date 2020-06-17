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
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 0;

    bool MorathiHighOracleOfKhaine::s_registered = false;

    MorathiHighOracleOfKhaine::MorathiHighOracleOfKhaine() :
            DaughterOfKhaine("Morathi, High Oracle of Khaine", 6, WOUNDS, 9, 4, false),
            m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 3, 3, 3, -1, RAND_D3),
            m_wings(Weapon::Type::Melee, "Bladed Wings", 2, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, WIZARD, MORATHI, HIGH_ORACLE_OF_KHAINE};
        m_weapons = {&m_heartrender, &m_wings};

        m_totalSpells = 3;
        m_totalUnbinds = 2;
    }

    bool MorathiHighOracleOfKhaine::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_heartrender);
        model->addMeleeWeapon(&m_wings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *MorathiHighOracleOfKhaine::Create(const ParameterList &parameters) {
        auto unit = new MorathiHighOracleOfKhaine();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

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
                            EnumParameter("Lore", g_lore[0], g_lore)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Morathi, High Oracle of Khaine", factoryMethod);
        }
    }

    int MorathiHighOracleOfKhaine::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = DaughterOfKhaine::targetHitModifier(weapon, attacker);

        // Enchanting Beauty
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
        return POINTS_PER_UNIT;
    }

} //namespace DaughtersOfKhaine

