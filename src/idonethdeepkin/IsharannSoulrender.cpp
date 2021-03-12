/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/IsharannSoulrender.h"
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool IsharannSoulrender::s_registered = false;

    Unit *IsharannSoulrender::Create(const ParameterList &parameters) {
        auto unit = new IsharannSoulrender();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_isharannArtefacts[0]);
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

    void IsharannSoulrender::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_isharannArtefacts[0], g_isharannArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Isharann Soulrender", factoryMethod);
        }
    }

    IsharannSoulrender::IsharannSoulrender() :
            IdonethDeepkinBase("Isharann Soulrender", 6, g_wounds, 8, 4, false),
            m_talunhook(Weapon::Type::Melee, "Talunhook", 2, 2, 3, 3, -1, 2),
            m_bill(Weapon::Type::Melee, "Rakerdart's Serrated Bill", 3, RAND_D3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, SOULRENDER};
        m_weapons = {&m_talunhook, &m_bill};
        m_battleFieldRole = Role::Leader;
    }

    bool IsharannSoulrender::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talunhook);
        model->addMeleeWeapon(&m_bill);

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int IsharannSoulrender::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //IdonethDeepkin