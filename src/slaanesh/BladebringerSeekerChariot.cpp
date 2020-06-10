/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BladebringerSeekerChariot.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Slaanesh {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    bool BladebringerOnSeekerChariot::s_registered = false;

    BladebringerOnSeekerChariot::BladebringerOnSeekerChariot() :
            SlaaneshBase("Bladebringer, Herald on Seeker Chariot", 12, WOUNDS, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SEEKER_CHARIOT,
                      BLADEBRINGER};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};

        // Impossibly Swift
        m_retreatAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool BladebringerOnSeekerChariot::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *BladebringerOnSeekerChariot::Create(const ParameterList &parameters) {
        auto unit = new BladebringerOnSeekerChariot();

        auto host = (Host) GetEnumParam("Host", parameters, Godseekers);
        unit->setHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BladebringerOnSeekerChariot::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BladebringerOnSeekerChariot::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    BladebringerOnSeekerChariot::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Bladebringer, Herald on Seeker Chariot", factoryMethod);
        }
    }

    void BladebringerOnSeekerChariot::onCharged() {
        Unit::onCharged();

        // Multilating Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::rollD6() >= 2) {
                ip->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    int BladebringerOnSeekerChariot::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
