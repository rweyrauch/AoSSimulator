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

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 80;

    bool MagoresFiends::s_registered = false;

    Unit *MagoresFiends::Create(const ParameterList &parameters) {
        return new MagoresFiends();
    }

    void MagoresFiends::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MagoresFiends::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    MagoresFiends::ComputePoints,
                    {
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Magore's Fiends", factoryMethod);
        }
    }

    MagoresFiends::MagoresFiends() :
            KhorneBase("Magore's Fiends", 5, g_wounds, 6, 4, false),
            m_bellyMaw(Weapon::Type::Melee, "Magore's Belly Maw", 1, 1, 4, 3, -1, RAND_D3),
            m_daemonicAxe(Weapon::Type::Melee, "Magore's Daemonic Axe", 1, 3, 3, 3, -1, 1),
            m_goreaxe(Weapon::Type::Melee, "Goreaxe", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, BLOOD_WARRIORS, MAGORES_FIENDS};
        m_weapons = {&m_bellyMaw, &m_daemonicAxe, &m_goreaxe};

        setSlaughterHost(SlaughterHost::Goretide);

        auto magore = new Model(g_basesize, wounds());
        magore->setName("Magore");
        magore->addMeleeWeapon(&m_bellyMaw);
        magore->addMeleeWeapon(&m_daemonicAxe);
        addModel(magore);

        auto ghartok = new Model(g_basesize, wounds());
        ghartok->setName("Ghartok");
        ghartok->addMeleeWeapon(&m_goreaxe);
        addModel(ghartok);

        auto zharkus = new Model(g_basesize, wounds());
        zharkus->setName("Zharkus");
        zharkus->addMeleeWeapon(&m_goreaxe);
        addModel(zharkus);

        m_points = g_pointsPerUnit;
    }

    Rerolls MagoresFiends::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (target->hasKeyword(STORMCAST_ETERNAL)) {
            return Rerolls::Failed;
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
            return Rerolls::Failed;
        }
        return KhorneBase::chargeRerolls();
    }

    int MagoresFiends::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MagoresFiends::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        KhorneBase::onFriendlyModelSlain(numSlain, attacker, source);

        // No Respite
        int numExtraSlain = 0;
        fight(1, m_meleeTarget, numExtraSlain);
    }

} // namespace Khorne
