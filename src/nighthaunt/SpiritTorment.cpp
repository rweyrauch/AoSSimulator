/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/SpiritTorment.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool SpiritTorment::s_registered = false;

    Unit *SpiritTorment::Create(const ParameterList &parameters) {
        auto unit = new SpiritTorment();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
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


    void SpiritTorment::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiritTorment::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    SpiritTorment::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Spirit Torment", factoryMethod);
        }
    }

    SpiritTorment::SpiritTorment() :
            Nighthaunt("Spirit Torment", 6, WOUNDS, 10, 4, true),
            m_chains(Weapon::Type::Melee, "Shacklegheist Chains", 2, 3, 4, 3, -2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, SPIRIT_TORMENT};
        m_weapons = {&m_chains};
    }

    bool SpiritTorment::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_chains);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    int SpiritTorment::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
