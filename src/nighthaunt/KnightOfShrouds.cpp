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
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool KnightOfShrouds::s_registered = false;

    Unit *KnightOfShrouds::Create(const ParameterList &parameters) {
        auto unit = new KnightOfShrouds();

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

    void KnightOfShrouds::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KnightOfShrouds::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KnightOfShrouds::ComputePoints,
                    {
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

    KnightOfShrouds::KnightOfShrouds() :
            Nighthaunt("Knight of Shrouds", 6, WOUNDS, 10, 4, true),
            m_sword(Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
        m_weapons = {&m_sword};
    }

    bool KnightOfShrouds::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    int KnightOfShrouds::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
