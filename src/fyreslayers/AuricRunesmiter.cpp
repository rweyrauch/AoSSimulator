/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiter.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool AuricRunesmiter::s_registered = false;

    AuricRunesmiter::AuricRunesmiter() :
            Fyreslayer("Auric Runesmiter", 4, WOUNDS, 7, 5, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1),
            m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 4, 3, 0, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, PRIEST, AURIC_RUNESMITER};
        m_weapons = {&m_throwingAxe, &m_runicIron, &m_latchAxe};
        m_battleFieldRole = Leader;
    }

    bool AuricRunesmiter::configure(Prayer prayer) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_latchAxe);
        model->addMeleeWeapon(&m_runicIron);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AuricRunesmiter::Create(const ParameterList &parameters) {
        auto unit = new AuricRunesmiter();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterSmiterTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_forgeTempleArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunesmiter::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunesmiter::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunesmiter::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_masterSmiterTraits[0], g_masterSmiterTraits),
                            EnumParameter("Artefact", g_forgeTempleArtefacts[0], g_forgeTempleArtefacts),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runesmiter", factoryMethod);
        }
    }

    int AuricRunesmiter::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Fyreslayers