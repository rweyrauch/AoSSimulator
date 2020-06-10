/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/MadcapShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    bool MadcapShaman::s_registered = false;

    MadcapShaman::MadcapShaman() :
            GloomspiteGitzBase("Madcap Shaman", 5, WOUNDS, 4, 6, false),
            m_moonStaff(Weapon::Type::Melee, "Moon Staff", 2, 1, 4, 4, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN};
        m_weapons = {&m_moonStaff};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool MadcapShaman::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_moonStaff);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<NightShroud>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *MadcapShaman::Create(const ParameterList &parameters) {
        auto unit = new MadcapShaman();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters,
                                                      (int) None);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
                        EnumParameter("Lore of the Moonclans", None, g_loreOfTheMoonclans)
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
        return ParameterValueToString(parameter);
    }

    int MadcapShaman::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) {
            return (int) lore;
        }
        return 0;
    }

    int MadcapShaman::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
