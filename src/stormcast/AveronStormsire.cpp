/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/AveronStormsire.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool AveronStormsire::s_registered = false;

    AveronStormsire::AveronStormsire(Lore lore, bool isGeneral) :
            StormcastEternal(Stormhost::None, "Averon Stormsire", 5, g_wounds, 9, 3, false),
            m_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO, WIZARD,
                      KNIGHT_INCANTOR, AVERON_STORMSIRE};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateStormsire(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateSpiritStorm(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *AveronStormsire::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AveronStormsire(lore, general);
     }

    void AveronStormsire::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Averon Stormsire", factoryMethod);
        }
    }

    std::string AveronStormsire::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int AveronStormsire::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void AveronStormsire::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int AveronStormsire::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int AveronStormsire::unbindingModifier() const {
        if (!m_usedVoidstormScroll) {
            m_usedVoidstormScroll = true;
            return Automatically_Unbound;
        }
        return Unit::unbindingModifier();
    }

} // namespace StormcastEternals