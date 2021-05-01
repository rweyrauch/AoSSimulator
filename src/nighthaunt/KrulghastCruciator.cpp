/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/KrulghastCruciator.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 130;

    bool KrulghastCruciator::s_registered = false;

    Unit *KrulghastCruciator::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KrulghastCruciator(trait, artefact, general);
    }

    void KrulghastCruciator::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KrulghastCruciator::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KrulghastCruciator::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")

                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Krulghast Cruciator", factoryMethod);
        }
    }

    KrulghastCruciator::KrulghastCruciator(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt("Krulghast Cruciator", 6, g_wounds, 10, 4, true) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, TOTEM, KRULGHAST_CRUCIATOR};
        m_weapons = {&m_torture, &m_talonsAndKnives};
        m_battleFieldRole = Role::Leader;
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_torture);
        model->addMeleeWeapon(&m_talonsAndKnives);
        addModel(model);

        m_points = ComputePoints(1);
    }

    int KrulghastCruciator::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
