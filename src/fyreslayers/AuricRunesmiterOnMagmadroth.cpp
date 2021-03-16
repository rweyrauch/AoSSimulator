/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiterOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 120; // x92 oval
    static const int g_pointsPerUnit = 250;

    bool AuricRunesmiterOnMagmadroth::s_registered = false;

    AuricRunesmiterOnMagmadroth::AuricRunesmiterOnMagmadroth() :
            Magmadroth("Auric Runesmiter on Magmadroth", 8),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 3, 3, 0, 2),
            m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, PRIEST, AURIC_RUNESMITER};
        m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_latchAxe, &m_runicIron};
        m_battleFieldRole = Role::Leader_Behemoth;
    }

    bool AuricRunesmiterOnMagmadroth::configure(Blessing prayer, MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_fyrestream);
        model->addMeleeWeapon(&m_clawsAndHorns);
        model->addMeleeWeapon(&m_blazingMaw);
        model->addMeleeWeapon(&m_latchAxe);
        model->addMeleeWeapon(&m_runicIron);
        addModel(model);

        m_prayer = prayer;

        m_mountTrait = trait;

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *AuricRunesmiterOnMagmadroth::Create(const ParameterList &parameters) {
        auto unit = new AuricRunesmiterOnMagmadroth();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterSmiterTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_forgeTempleArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Blessing) GetEnumParam("Prayer", parameters, g_prayers[0]);
        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_mountTraits[0]);

        bool ok = unit->configure(prayer, mount);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunesmiterOnMagmadroth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunesmiterOnMagmadroth::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunesmiterOnMagmadroth::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_masterSmiterTraits[0], g_masterSmiterTraits),
                            EnumParameter("Artefact", g_forgeTempleArtefacts[0], g_forgeTempleArtefacts),
                            EnumParameter("Mount Trait", g_mountTraits[0], g_mountTraits),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runesmiter on Magmadroth", factoryMethod);
        }
    }

    int AuricRunesmiterOnMagmadroth::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers