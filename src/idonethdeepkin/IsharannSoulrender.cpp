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
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool IsharannSoulrender::s_registered = false;

    Unit *IsharannSoulrender::Create(const ParameterList &parameters) {
        auto unit = new IsharannSoulrender();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_isharannArtefacts[0]);
        unit->setArtefact(artefact);

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
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Isharann Soulrender", factoryMethod);
        }
    }

    IsharannSoulrender::IsharannSoulrender() :
            IdonethDeepkinBase("Isharann Soulrender", 6, WOUNDS, 8, 4, false),
            m_talunhook(Weapon::Type::Melee, "Talunhook", 2, 2, 3, 3, -1, 2),
            m_bill(Weapon::Type::Melee, "Rakerdart's Serrated Bill", 3, RAND_D3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, SOULRENDER};
        m_weapons = {&m_talunhook, &m_bill};
    }

    bool IsharannSoulrender::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_talunhook);
        model->addMeleeWeapon(&m_bill);

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int IsharannSoulrender::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //IdonethDeepkin