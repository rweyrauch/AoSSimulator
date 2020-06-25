/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "idonethdeepkin/IsharannTidecaster.h"
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool IsharannTidecaster::s_registered = false;

    Unit *IsharannTidecaster::Create(const ParameterList &parameters) {
        auto unit = new IsharannTidecaster();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaneArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void IsharannTidecaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_arcaneArtefacts[0], g_arcaneArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Isharann Tidecaster", factoryMethod);
        }
    }

    IsharannTidecaster::IsharannTidecaster() :
            IdonethDeepkinBase("Isharann Tidecaster", 6, WOUNDS, 7, 6, false),
            m_staff(Weapon::Type::Melee, "Pelagic Staff", 1, 2, 3, 3, 0, RAND_D3) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, WIZARD, TIDECASTER};
        m_weapons = {&m_staff};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool IsharannTidecaster::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int IsharannTidecaster::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //IdonethDeepkin