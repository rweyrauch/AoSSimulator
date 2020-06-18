/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/AetherKhemist.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    bool AetherKhemist::s_registered = false;

    Unit *AetherKhemist::Create(const ParameterList &parameters) {
        auto unit = new AetherKhemist();

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_khemistCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_khemistArtefacts[0]);
        unit->setArtefact(artefact);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AetherKhemist::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_khemistCommandTraits[0], g_khemistCommandTraits),
                            EnumParameter("Artefact", g_khemistArtefacts[0], g_khemistArtefacts)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };

            s_registered = UnitFactory::Register("Aether Khemist", factoryMethod);
        }
    }

    AetherKhemist::AetherKhemist() :
            KharadronBase("Aether Khemist", 4, WOUNDS, 7, 4, false),
            m_anatomiser(Weapon::Type::Missile, "Atmospheric Anatomiser", 9, RAND_3D6, 4, 4, -2, 1),
            m_instruments(Weapon::Type::Melee, "Heavy Instruments", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, AETHER_KHEMIST};
        m_weapons = {&m_anatomiser, &m_instruments};

        s_globalToHitMod.connect(this, &AetherKhemist::atmosphericIsolation, &m_connection);
    }

    AetherKhemist::~AetherKhemist() {
        m_connection.disconnect();
    }

    bool AetherKhemist::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_anatomiser);
        model->addMeleeWeapon(&m_instruments);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int AetherKhemist::atmosphericIsolation(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *target) {
        // Atmospheric Isolation
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int AetherKhemist::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //KharadronOverlords
