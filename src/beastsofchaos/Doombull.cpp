/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Doombull.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 100;

    bool Doombull::s_registered = false;

    Doombull::Doombull() :
            BeastsOfChaosBase("Doombull", 8, g_wounds, 7, 5, false),
            m_bullgorHorns(Weapon::Type::Melee, "Bullgor Horns", 1, 2, 4, 4, 0, 1),
            m_slaughtererAxe(Weapon::Type::Melee, "Slaughterer's Axe", 1, 3, 3, 3, -2, 3) {
        m_keywords = {CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, HERO, DOOMBULL};
        m_weapons = {&m_bullgorHorns, &m_slaughtererAxe};
        m_battleFieldRole = Role::Leader;
    }

    bool Doombull::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bullgorHorns);
        model->addMeleeWeapon(&m_slaughtererAxe);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Slaughter's Call", 12, 12, Phase::Combat,
                                                             Attribute::To_Wound_Melee, 1,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>{WARHERD}));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Doombull::Create(const ParameterList &parameters) {
        auto unit = new Doombull();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_warherdCommandTrait[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_warherdArtefact[0]);
        unit->setArtefact(artefact);
        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Doombull::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                            EnumParameter("Command Trait", g_warherdCommandTrait[0], g_warherdCommandTrait),
                            EnumParameter("Artefact", g_warherdArtefact[0], g_warherdArtefact),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Doombull", factoryMethod);
        }
    }

    Wounds Doombull::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Bloodgreed
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Doombull::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
