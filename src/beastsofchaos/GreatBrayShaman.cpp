/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/GreatBrayShaman.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool GreatBrayShaman::s_registered = false;

    GreatBrayShaman::GreatBrayShaman() :
            BeastsOfChaosBase("Great Bray-shaman", 6, g_wounds, 6, 6, false),
            m_fetishStaff(Weapon::Type::Melee, "Fetish Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, WIZARD, GREAT_BRAY_SHAMAN};
        m_weapons = {&m_fetishStaff};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        s_globalRunMod.connect(this, &GreatBrayShaman::infuseWithBestialVigour, &m_connection);
    }

    GreatBrayShaman::~GreatBrayShaman() {
        m_connection.disconnect();
    }

    bool GreatBrayShaman::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_fetishStaff);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *GreatBrayShaman::Create(const ParameterList &parameters) {
        auto unit = new GreatBrayShaman();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_brayherdCommandTrait[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_brayherdArtefact[0]);
        unit->setArtefact(artefact);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheTwistedWilds[0]);
        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GreatBrayShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                            EnumParameter("Command Trait", g_brayherdCommandTrait[0], g_brayherdCommandTrait),
                            EnumParameter("Artefact", g_brayherdArtefact[0], g_brayherdArtefact),
                            EnumParameter("Lore", g_loreOfTheTwistedWilds[0], g_loreOfTheTwistedWilds),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Great Bray-shaman", factoryMethod);
        }
    }

    int GreatBrayShaman::infuseWithBestialVigour(const Unit *unit) {
        // Infuse with Bestial Vigour
        if (unit->hasKeyword(BRAYHERD) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return 3;
        }
        return 0;
    }

    int GreatBrayShaman::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
