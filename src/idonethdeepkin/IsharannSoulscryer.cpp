/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/IsharannSoulscryer.h"
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    bool IsharannSoulscryer::s_registered = false;

    Unit *IsharannSoulscryer::Create(const ParameterList &parameters) {
        auto unit = new IsharannSoulscryer();

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

    void IsharannSoulscryer::Init() {
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

            s_registered = UnitFactory::Register("Isharann Soulscryer", factoryMethod);
        }
    }

    IsharannSoulscryer::IsharannSoulscryer() :
            IdonethDeepkinBase("Isharann Soulscryer", 6, WOUNDS, 7, 6, false),
            m_shoal(Weapon::Type::Missile, "Scryfish Shoal", 18, 8, 5, 5, 0, 1),
            m_claw(Weapon::Type::Melee, "Finger-claw", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, PRIEST, SOULSCRYER};
        m_weapons = {&m_shoal, &m_claw};
        m_battleFieldRole = Leader;
    }

    bool IsharannSoulscryer::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_shoal);
        model->addMeleeWeapon(&m_claw);

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int IsharannSoulscryer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //IdonethDeepkin