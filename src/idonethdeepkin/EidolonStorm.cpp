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
        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_idonethArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new EidolonOfMathlannAspectOfTheStorm(enclave, trait, artefact, general);
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

    EidolonOfMathlannAspectOfTheStorm::EidolonOfMathlannAspectOfTheStorm(Enclave enclave, CommandTrait trait, Artefact artefact, bool isGeneral) :
            IdonethDeepkinBase(enclave, "Eidolon of Mathlann Aspect of the Storm", 12, g_wounds, 10, 3, true, g_pointsPerUnit),
            m_spear(Weapon::Type::Melee, "Spear of Repressed Fury", 2, 4, 3, 2, -2, 2),
            m_crulhook(Weapon::Type::Melee, "Crulhook", 1, 4, 3, 2, -1, 1),
            m_fangs(Weapon::Type::Melee, "Sharp Fangs", 3, RAND_2D6, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, ASPECT_OF_THE_STORM};
        m_weapons = {&m_spear, &m_crulhook, &m_fangs};
        m_battleFieldRole = Role::Leader;
        m_retreatAndCharge = true;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_crulhook);
        model->addMeleeWeapon(&m_fangs);

        addModel(model);
    }

    Rerolls EidolonOfMathlannAspectOfTheStorm::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Crashing Upon the Foe
        if (m_charged && (weapon->name() == m_spear.name())) {
            return Rerolls::Ones;
        }

        return IdonethDeepkinBase::toHitRerolls(weapon, target);
    }

    Wounds EidolonOfMathlannAspectOfTheStorm::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                           int woundRoll) const {
        // Crashing Upon the Foe
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {weapon->damage() + 1, 0};
        }

        return IdonethDeepkinBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void EidolonOfMathlannAspectOfTheStorm::onCharged() {
        IdonethDeepkinBase::onCharged();

        // Crashing Upon the Foe
        if (m_charged) {
            heal(Dice::RollD3());
        }
    }

    int EidolonOfMathlannAspectOfTheStorm::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // IdonethDeepkin

