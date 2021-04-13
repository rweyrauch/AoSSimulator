/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/EidolonStorm.h"
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 100;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 330;

    bool EidolonOfMathlannAspectOfTheStorm::s_registered = false;

    Unit *EidolonOfMathlannAspectOfTheStorm::Create(const ParameterList &parameters) {
        auto unit = new EidolonOfMathlannAspectOfTheStorm();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_idonethArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void EidolonOfMathlannAspectOfTheStorm::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_idonethArtefacts[0], g_idonethArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Eidolon of Mathlann Aspect of the Storm", factoryMethod);
        }
    }

    EidolonOfMathlannAspectOfTheStorm::EidolonOfMathlannAspectOfTheStorm() :
            IdonethDeepkinBase("Eidolon of Mathlann Aspect of the Storm", 12, g_wounds, 10, 3, true),
            m_spear(Weapon::Type::Melee, "Spear of Repressed Fury", 2, 4, 3, 2, -2, 2),
            m_crulhook(Weapon::Type::Melee, "Crulhook", 1, 4, 3, 2, -1, 1),
            m_fangs(Weapon::Type::Melee, "Sharp Fangs", 3, RAND_2D6, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, ASPECT_OF_THE_STORM};
        m_weapons = {&m_spear, &m_crulhook, &m_fangs};
        m_battleFieldRole = Role::Leader;
        m_retreatAndCharge = true;
    }

    void EidolonOfMathlannAspectOfTheStorm::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_crulhook);
        model->addMeleeWeapon(&m_fangs);

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Rerolls EidolonOfMathlannAspectOfTheStorm::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Crashing Upon the Foe
        if (m_charged && (weapon->name() == m_spear.name())) {
            return Rerolls::Ones;
        }

        return Unit::toHitRerolls(weapon, target);
    }

    Wounds EidolonOfMathlannAspectOfTheStorm::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                           int woundRoll) const {
        // Crashing Upon the Foe
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {weapon->damage() + 1, 0};
        }

        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void EidolonOfMathlannAspectOfTheStorm::onCharged() {
        Unit::onCharged();

        // Crashing Upon the Foe
        if (m_charged) {
            heal(Dice::RollD3());
        }
    }

    int EidolonOfMathlannAspectOfTheStorm::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // IdonethDeepkin

