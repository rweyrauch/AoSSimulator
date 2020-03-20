/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodThrone.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    bool HeraldOfKhorneOnBloodThrone::s_registered = false;

    HeraldOfKhorneOnBloodThrone::HeraldOfKhorneOnBloodThrone() :
            KhorneBase("Herald of Khorne on Blood Throne", 8, WOUNDS, 10, 4, false),
            m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1),
            m_hellblades(Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1),
            m_gnashingMaw(Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTER, HERO, HERALD_OF_KHORNE, HERALD_OF_KHORNE_ON_BLOOD_THRONE};
        m_weapons = {&m_bladeOfBlood, &m_hellblades, &m_gnashingMaw};
    }

    bool HeraldOfKhorneOnBloodThrone::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        model->addMeleeWeapon(&m_hellblades);
        model->addMeleeWeapon(&m_gnashingMaw);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *HeraldOfKhorneOnBloodThrone::Create(const ParameterList &parameters) {
        auto unit = new HeraldOfKhorneOnBloodThrone();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HeraldOfKhorneOnBloodThrone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HeraldOfKhorneOnBloodThrone::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    HeraldOfKhorneOnBloodThrone::ComputePoints,
                    {
                            {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None,
                             KhorneBase::SkullfiendTribe, 1}
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Herald of Khorne on Blood Throne", factoryMethod);
        }
    }

    Wounds HeraldOfKhorneOnBloodThrone::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int HeraldOfKhorneOnBloodThrone::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne