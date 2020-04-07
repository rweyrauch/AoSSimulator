/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SeekerChariots.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Slaanesh {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool SeekerChariots::s_registered = false;

    SeekerChariots::SeekerChariots() :
            SlaaneshBase("Seeker Chariots", 12, WOUNDS, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, SEEKER_CHARIOTS};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};

        // Impossibly Swift
        m_retreatAndCharge = true;
    }

    bool SeekerChariots::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_flensingWhips);
            model->addMeleeWeapon(&m_piercingClaws);
            model->addMeleeWeapon(&m_poisonedTongues);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SeekerChariots::Create(const ParameterList &parameters) {
        auto unit = new SeekerChariots();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto host = (Host) GetEnumParam("Host", parameters, Godseekers);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SeekerChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SeekerChariots::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    SeekerChariots::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders,
                             SlaaneshBase::Godseekers, 1},
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Seeker Chariots", factoryMethod);
        }
    }

    void SeekerChariots::onCharged() {
        Unit::onCharged();

        // Multilating Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto ip : units) {
            if (Dice::rollD6() >= 2) {
                ip->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    int SeekerChariots::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // Slannesh
