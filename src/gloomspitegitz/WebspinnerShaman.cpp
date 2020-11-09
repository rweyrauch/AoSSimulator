/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/WebspinnerShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 80;

    bool WebspinnerShaman::s_registered = false;

    WebspinnerShaman::WebspinnerShaman() :
            GloomspiteGitzBase("Webspinner Shaman", 5, g_wounds, 4, 6, false),
            m_spiderGodStaff(Weapon::Type::Melee, "Spider God Staff", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, WIZARD, WEBSPINNER_SHAMAN};
        m_weapons = {&m_spiderGodStaff};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool WebspinnerShaman::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spiderGodStaff);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<SpeedOfTheSpiderGod>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *WebspinnerShaman::Create(const ParameterList &parameters) {
        auto unit = new WebspinnerShaman();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheSpiderFangs[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_giftsOfTheGloomspite[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_venomousValuables[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WebspinnerShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WebspinnerShaman::Create,
                    WebspinnerShaman::ValueToString,
                    WebspinnerShaman::EnumStringToInt,
                    WebspinnerShaman::ComputePoints,
                    {
                        EnumParameter("Lore of the Spiderfangs", g_loreOfTheSpiderFangs[0], g_loreOfTheSpiderFangs),
                            EnumParameter("Command Trait", g_giftsOfTheGloomspite[0], g_giftsOfTheGloomspite),
                            EnumParameter("Artefact", g_venomousValuables[0], g_venomousValuables),
                        BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Webspinner Shaman", factoryMethod);
        }
    }

    std::string WebspinnerShaman::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Spiderfangs") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int WebspinnerShaman::EnumStringToInt(const std::string &enumString) {
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int WebspinnerShaman::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
