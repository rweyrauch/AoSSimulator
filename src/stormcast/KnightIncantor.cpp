/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightIncantor.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool KnightIncantor::s_registered = false;

    KnightIncantor::KnightIncantor() :
            StormcastEternal("Knight-Incantor", 5, WOUNDS, 9, 3, false),
            m_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, KNIGHT_INCANTOR};
        m_weapons = {&m_staff};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool KnightIncantor::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration) {
        if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None)) {
            return false;
        }

        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateSpiritStorm(this)));
        if (storm != LoreOfTheStorm::None)
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
        if (invigoration != LoreOfInvigoration::None)
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KnightIncantor::Create(const ParameterList &parameters) {
        auto unit = new KnightIncantor();
        auto storm = (LoreOfTheStorm) GetEnumParam("Lore of the Storm", parameters, (int) LoreOfTheStorm::None);
        auto invigoration = (LoreOfInvigoration) GetEnumParam("Lore of Invigoration", parameters,
                                                              (int) LoreOfInvigoration::None);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(storm, invigoration);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KnightIncantor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Lore of the Storm", (int) LoreOfTheStorm::None,
                             (int) LoreOfTheStorm::None, (int) LoreOfTheStorm::Stormcaller, 1},
                            {ParamType::Enum, "Lore of Invigoration", (int) LoreOfInvigoration::None,
                             (int) LoreOfInvigoration::None, (int) LoreOfInvigoration::SpeedOfLightning, 1},
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Incantor", factoryMethod);
        }
    }

    std::string KnightIncantor::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Storm") {
            return ToString((LoreOfTheStorm) parameter.intValue);
        } else if (std::string(parameter.name) == "Lore of Invigoration") {
            return ToString((LoreOfInvigoration) parameter.intValue);
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int KnightIncantor::EnumStringToInt(const std::string &enumString) {
        LoreOfTheStorm storm;
        LoreOfInvigoration invigoration;
        if (FromString(enumString, storm)) {
            return (int) storm;
        } else if (FromString(enumString, invigoration)) {
            return (int) invigoration;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void KnightIncantor::onStartCombat(PlayerId player) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int KnightIncantor::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals