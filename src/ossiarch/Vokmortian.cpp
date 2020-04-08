/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Vokmortian.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 180;

    bool Vokmortian::s_registered = false;

    Unit *Vokmortian::Create(const ParameterList &parameters) {
        auto unit = new Vokmortian();

        auto legion = (Legion) GetEnumParam("Legion", parameters, None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Vokmortian::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int Vokmortian::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void Vokmortian::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None,
                             OssiarchBonereaperBase::Crematorians, 1},
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };

            s_registered = UnitFactory::Register("Vokmortian", factoryMethod);
        }
    }

    Vokmortian::Vokmortian() :
            OssiarchBonereaperBase("Vokmortian", 5, WOUNDS, 10, 5, false),
            m_gazeOfDeath(Weapon::Type::Missile, "Gaze of Death", 12, 1, 3, 2, -1, RAND_D3),
            m_staff(Weapon::Type::Melee, "Staff of Retribution", 2, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, HERO, WIZARD, VOKMORTIAN};
        m_weapons = {&m_gazeOfDeath, &m_staff};

        s_globalBraveryMod.connect(this, &Vokmortian::grimWarning, &m_connection);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    Vokmortian::~Vokmortian() {
        m_connection.disconnect();
    }

    bool Vokmortian::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_gazeOfDeath);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Vokmortian::grimWarning(const Unit *unit) {
        // Grim Warning
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int Vokmortian::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace OssiarchBonereapers
