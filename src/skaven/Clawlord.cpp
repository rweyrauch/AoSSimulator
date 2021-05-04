/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Clawlord.h>
#include <UnitFactory.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Clawlord::s_registered = false;

    Unit *Clawlord::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_verminousCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_verminousArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Clawlord(trait, artefact, general);
    }

    void Clawlord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_verminousCommandTraits[0], g_verminousCommandTraits),
                            EnumParameter("Artefact", g_verminousArtefacts[0], g_verminousArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Clawlord", factoryMethod);
        }
    }

    Clawlord::Clawlord(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Clawlord", 6, g_wounds, 6, 4, false, g_pointsPerUnit),
            m_blade(Weapon::Type::Melee, "Warpforged Blade", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, HERO, CLAWLORD};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        addModel(model);
    }

    int Clawlord::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

        // Cornered Fury
        extra += (initialWounds() - remainingWounds());

        return extra;
    }

    int Clawlord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }
} //namespace Skaven
