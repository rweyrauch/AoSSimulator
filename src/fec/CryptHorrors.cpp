/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptHorrors.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 130 * 4;

    bool CryptHorrors::s_registered = false;

    CryptHorrors::CryptHorrors() :
            FleshEaterCourts("Crypt Horrors", 7, WOUNDS, 10, 5, false),
            m_clubsAndTalons(Weapon::Type::Melee, "Clubs and Septic Talons", 1, 3, 4, 3, 0, 2),
            m_clubsAndTalonsHaunter(Weapon::Type::Melee, "Clubs and Septic Talons", 1, 4, 4, 3, 0, 2) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, KNIGHTS, CRYPT_HORRORS};
        m_weapons = {&m_clubsAndTalons, &m_clubsAndTalonsHaunter};
    }

    bool CryptHorrors::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto haunter = new Model(BASESIZE, wounds());
        haunter->addMeleeWeapon(&m_clubsAndTalonsHaunter);
        addModel(haunter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_clubsAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *CryptHorrors::Create(const ParameterList &parameters) {
        auto unit = new CryptHorrors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, NoCourt);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, None);
        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == NoCourt)
            unit->setCourtsOfDelusion(delusion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CryptHorrors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CryptHorrors::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    CryptHorrors::ComputePoints,
                    {
                            IntegerParameter( "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Crypt Horrors", factoryMethod);
        }
    }

    Rerolls CryptHorrors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Chosen of the King
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 18.0);
        if (unit != nullptr) {
            return RerollFailed;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    void CryptHorrors::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Noble Blood
            int woundsHealed = heal(remainingModels());
            if (woundsHealed) {
                //std::cout << "Healed " << woundsHealed << " from Noble Blood." << std::endl;
            }
        }
    }

    Wounds CryptHorrors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Warrior Elite - one additional damage for each wound roll of 6.
        if (woundRoll == 6) {
            return {weapon->damage() + 1, 0};
        }
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int CryptHorrors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace FleshEaterCourt
