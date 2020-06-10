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

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    bool WebspinnerShaman::s_registered = false;

    WebspinnerShaman::WebspinnerShaman() :
            GloomspiteGitzBase("Webspinner Shaman", 5, WOUNDS, 4, 6, false),
            m_spiderGodStaff(Weapon::Type::Melee, "Spider God Staff", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, WIZARD, WEBSPINNER_SHAMAN};
        m_weapons = {&m_spiderGodStaff};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool WebspinnerShaman::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_spiderGodStaff);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<SpeedOfTheSpiderGod>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *WebspinnerShaman::Create(const ParameterList &parameters) {
        auto unit = new WebspinnerShaman();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters,
                                                        (int) None);

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
                        EnumParameter("Lore of the Spiderfangs", None, g_loreOfTheSpiderFangs)
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
        return ParameterValueToString(parameter);
    }

    int WebspinnerShaman::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) {
            return (int) lore;
        }
        return 0;
    }

    int WebspinnerShaman::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
