/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullmaster.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne {
    static const int BASESIZE = 75; // x50 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool Skullmaster::s_registered = false;

    Skullmaster::Skullmaster() :
            KhorneBase("Skullmaster", 8, WOUNDS, 10, 4, false),
            m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1),
            m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, SKULLMASTER};
        m_weapons = {&m_bladeOfBlood, &m_brazenHooves};
    }

    bool Skullmaster::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bladeOfBlood);
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Skullmaster::Create(const ParameterList &parameters) {
        auto unit = new Skullmaster();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, Khorne::None);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Skullmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skullmaster::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Skullmaster::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skullmaster", factoryMethod);
        }
    }

    Wounds Skullmaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Skullmaster::onCharged() {
        // Murderous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::rollD6(remainingModels(), rolls);
            Wounds wounds = {0, 0};
            if (remainingModels() >= 6) {
                for (int i = 0; i < rolls.rollsGE(2); i++) {
                    wounds.mortal += Dice::rollD3();
                }
            } else {
                wounds.mortal = rolls.rollsGE(2);
            }

            SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                   name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds);
        }
        KhorneBase::onCharged();
    }

    Rerolls Skullmaster::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Slaughter and Ruin
        if (m_charged) {
            return RerollFailed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    int Skullmaster::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne
