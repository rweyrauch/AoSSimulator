/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/MadcapShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 80;

    bool MadcapShaman::s_registered = false;

    MadcapShaman::MadcapShaman() :
            GloomspiteGitzBase("Madcap Shaman", 5, g_wounds, 4, 6, false, g_pointsPerUnit),
            m_moonStaff(Weapon::Type::Melee, "Moon Staff", 2, 1, 4, 4, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN};
        m_weapons = {&m_moonStaff};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    void MadcapShaman::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_moonStaff);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Night Shroud", 5, 12,
                                                                    Attribute::Target_To_Hit_Missile, -1,
                                                                    Abilities::Target::SelfAndFriendly));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        if (m_commandTrait == CommandTrait::Boss_Shaman) {
            m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateImDaBossNoStabEmGood(this)));
        }

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *MadcapShaman::Create(const ParameterList &parameters) {
        auto unit = new MadcapShaman();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheMoonclans[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_giftsOfTheGloomspite[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_foetidFetishes[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure(lore);
        return unit;
    }

    void MadcapShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MadcapShaman::Create,
                    MadcapShaman::ValueToString,
                    MadcapShaman::EnumStringToInt,
                    MadcapShaman::ComputePoints,
                    {
                            EnumParameter("Lore of the Moonclans", g_loreOfTheMoonclans[0], g_loreOfTheMoonclans),
                            EnumParameter("Command Trait", g_giftsOfTheGloomspite[0], g_giftsOfTheGloomspite),
                            EnumParameter("Artefact", g_foetidFetishes[0], g_foetidFetishes),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Madcap Shaman", factoryMethod);
        }
    }

    std::string MadcapShaman::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Moonclans") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int MadcapShaman::EnumStringToInt(const std::string &enumString) {
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int MadcapShaman::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
