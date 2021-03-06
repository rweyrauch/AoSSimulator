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
#include "LoreOfShadows.h"
#include "DoKCommands.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 210;

    bool MorathiHighOracleOfKhaine::s_registered = false;

    MorathiHighOracleOfKhaine::MorathiHighOracleOfKhaine(Temple temple, Lore lore, bool isGeneral) :
            DaughterOfKhaine(temple, "Morathi-Khaine", 6, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 3, 3, 3, -1, RAND_D3),
            m_wings(Weapon::Type::Melee, "Bladed Wings", 2, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, WIZARD, MORATHI_KHAINE};
        m_weapons = {&m_heartrender, &m_wings};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 3;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_heartrender);
        model->addMeleeWeapon(&m_wings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        configureCommon();
    }

    Unit *MorathiHighOracleOfKhaine::Create(const ParameterList &parameters) {
        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new MorathiHighOracleOfKhaine(temple, lore, general);
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
        auto mod = DaughterOfKhaine::castingModifier();
        // Sorceress Supreme
        mod++;
        return mod;
    }

    int MorathiHighOracleOfKhaine::unbindingModifier() const {
        auto mod = DaughterOfKhaine::unbindingModifier();
        // Sorceress Supreme
        mod++;
        return mod;
    }

    Wounds MorathiHighOracleOfKhaine::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = DaughterOfKhaine::applyWoundSave(wounds, attackingUnit);

        return totalWounds;
    }

    int MorathiHighOracleOfKhaine::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace DaughtersOfKhaine

