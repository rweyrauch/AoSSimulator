/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/FungoidCaveShaman.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <iostream>
#include <spells/MysticShield.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 90;

    bool FungoidCaveShaman::s_registered = false;

    FungoidCaveShaman::FungoidCaveShaman() :
            GloomspiteGitzBase("Fungoid Cave-shaman", 5, g_wounds, 4, 6, false),
            m_moonSickle(Weapon::Type::Melee, "Moon-sickle", 1, 3, 4, 4, -1, 1),
            m_squigsTeeth(Weapon::Type::Melee, "Spore Squig's Vicious Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, FUNGOID_CAVE_SHAMAN};
        m_weapons = {&m_moonSickle, &m_squigsTeeth};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool FungoidCaveShaman::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_moonSickle);
        model->addMeleeWeapon(&m_squigsTeeth);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::make_unique<AreaOfEffectSpell>(this, "Spore Maws", 7, 0, RAND_D6, RAND_D3, 0));

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *FungoidCaveShaman::Create(const ParameterList &parameters) {
        auto unit = new FungoidCaveShaman();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheMoonclans[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_giftsOfTheGloomspite[0]);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void FungoidCaveShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore of the Moonclans", g_loreOfTheMoonclans[0], g_loreOfTheMoonclans),
                            EnumParameter("Command Trait", g_giftsOfTheGloomspite[0], g_giftsOfTheGloomspite),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Fungoid Cave-shaman", factoryMethod);
        }
    }

    std::string FungoidCaveShaman::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Moonclans") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int FungoidCaveShaman::EnumStringToInt(const std::string &enumString) {
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    void FungoidCaveShaman::onStartHero(PlayerId playerId) {
        GloomspiteGitzBase::onStartHero(playerId);

        // Mouthpiece of Mork
        if ((owningPlayer() == playerId) && m_roster) {
            if (Dice::RollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }
    }

    int FungoidCaveShaman::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Wounds FungoidCaveShaman::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Spore Squig
        return ignoreWounds(wounds, 4);
    }

} // namespace GloomspiteGitz
