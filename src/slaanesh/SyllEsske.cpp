/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SyllEsske.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace Slaanesh {
    static const int BASESIZE = 25;
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 120;

    bool SyllEsske::s_registered = false;

    SyllEsske::SyllEsske() :
            SlaaneshBase("Syll'Esske The Vengeful Allegiance", 8, WOUNDS, 10, 4, false),
            m_axeOfDominion(Weapon::Type::Melee, "Axe of Dominion", 2, 4, 4, 3, -2, RAND_D3),
            m_scourgingWhip(Weapon::Type::Melee, "Scourging Whip", 2, 8, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SYLL_ESSKE,
                      THE_VENGEFUL_ALLEGIANCE};
        m_weapons = {&m_axeOfDominion, &m_scourgingWhip};

        // Lithe and Swift
        m_runAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SyllEsske::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_axeOfDominion);
        model->addMeleeWeapon(&m_scourgingWhip);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *SyllEsske::Create(const ParameterList &parameters) {
        auto unit = new SyllEsske();

        auto host = (Host) GetEnumParam("Host", parameters, Godseekers);
        unit->setHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SyllEsske::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SyllEsske::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    SyllEsske::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Syll'Esske The Vengeful Allegiance", factoryMethod);
        }
    }

    int SyllEsske::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
