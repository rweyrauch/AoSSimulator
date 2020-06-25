/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/EndrinmasterWithHarness.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool EndrinmasterWithEndrinharness::s_registered = false;

    Unit *EndrinmasterWithEndrinharness::Create(const ParameterList &parameters) {
        auto unit = new EndrinmasterWithEndrinharness();

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_endrinmasterCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_endrinmasterArtefacts[0]);
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

    void EndrinmasterWithEndrinharness::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EndrinmasterWithEndrinharness::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    EndrinmasterWithEndrinharness::ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_endrinmasterCommandTraits[0], g_endrinmasterCommandTraits),
                            EnumParameter("Artefact", g_endrinmasterArtefacts[0], g_endrinmasterArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Endrinmaster with Endrinharness", factoryMethod);
        }
    }

    EndrinmasterWithEndrinharness::EndrinmasterWithEndrinharness() :
            KharadronBase("Endrinmaster with Endrinharness", 4, WOUNDS, 7, 4, false),
            m_gaze(Weapon::Type::Missile, "Gaze of Grungni", 9, 1, 3, 2, -1, RAND_D3),
            m_hammer(Weapon::Type::Melee, "Aethermight Hammer", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, ENDRINMASTER};
        m_weapons = {&m_gaze, &m_hammer};
    }

    bool EndrinmasterWithEndrinharness::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_hammer);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int EndrinmasterWithEndrinharness::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //KharadronOverlords
