/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MagoresFiends.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 80;

    bool MagoresFiends::s_registered = false;

    Unit *MagoresFiends::Create(const ParameterList &parameters) {
        auto unit = new MagoresFiends();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MagoresFiends::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MagoresFiends::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    MagoresFiends::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Magore's Fiends", factoryMethod);
        }
    }

    MagoresFiends::MagoresFiends() :
            KhorneBase("Magore's Fiends", 5, WOUNDS, 6, 4, false),
            m_bellyMaw(Weapon::Type::Melee, "Magore's Belly Maw", 1, 1, 4, 3, -1, RAND_D3),
            m_daemonicAxe(Weapon::Type::Melee, "Magore's Daemonic Axe", 1, 3, 3, 3, -1, 1),
            m_goreaxe(Weapon::Type::Melee, "Goreaxe", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, BLOOD_WARRIORS, MAGORES_FIENDS};
        m_weapons = {&m_bellyMaw, &m_daemonicAxe, &m_goreaxe};
    }

    bool MagoresFiends::configure() {
        auto magore = new Model(BASESIZE, wounds());
        magore->setName("Magore");
        magore->addMeleeWeapon(&m_bellyMaw);
        magore->addMeleeWeapon(&m_daemonicAxe);
        addModel(magore);

        auto ghartok = new Model(BASESIZE, wounds());
        ghartok->setName("Ghartok");
        ghartok->addMeleeWeapon(&m_goreaxe);
        addModel(ghartok);

        auto zharkus = new Model(BASESIZE, wounds());
        zharkus->setName("Zharkus");
        zharkus->addMeleeWeapon(&m_goreaxe);
        addModel(zharkus);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Rerolls MagoresFiends::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (target->hasKeyword(STORMCAST_ETERNAL)) {
            return RerollFailed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    Wounds MagoresFiends::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = KhorneBase::computeReturnedDamage(weapon, saveRoll);
        // Gorefists
        if (saveRoll == 6) {
            wounds += {0, 1};
        }
        return wounds;
    }

    Rerolls MagoresFiends::chargeRerolls() const {
        // Blood Scent
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), RIPTOOTH, 3.0);
        if (unit) {
            return RerollFailed;
        }
        return Unit::chargeRerolls();
    }

    int MagoresFiends::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne
