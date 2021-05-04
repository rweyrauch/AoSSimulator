/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/KnightOfShrouds.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool KnightOfShrouds::s_registered = false;

    Unit *KnightOfShrouds::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightOfShrouds(procession, trait, artefact, general);
    }

    void KnightOfShrouds::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KnightOfShrouds::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KnightOfShrouds::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Knight of Shrouds", factoryMethod);
        }
    }

    KnightOfShrouds::KnightOfShrouds(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt(procession, "Knight of Shrouds", 6, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
        m_weapons = {&m_sword};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        addModel(model);
    }

    int KnightOfShrouds::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void
    KnightOfShrouds::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon, const Wounds &weaponDamage) {
        Nighthaunt::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Stolen Hours
        if (enemyUnit->hasKeyword(HERO) && (weapon->name() == m_sword.name())) {
            heal(1);
        }
    }

} // namespace Nighthaunt
