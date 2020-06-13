/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/VorgarothAndSkalok.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 280; // x210 oval
    static const int WOUNDS = 30;
    static const int POINTS_PER_UNIT = 1200;

    struct TableEntry {
        int m_move;
        int m_clawsRend;
        int m_jawsToWound;
        int m_tailAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {5, 10, 15, 20, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, -3, 2, RAND_3D6},
                    {11, -3, 3, RAND_2D6},
                    {10, -2, 4, RAND_D6},
                    {9,  -1, 5, RAND_D3},
                    {8,  -1, 6, 1}
            };

    bool VorgarothAndSkalok::s_registered = false;

    VorgarothAndSkalok::VorgarothAndSkalok() :
            KhorneBase("Vorgaroth the Scarred & Skalok the Skull Host of Khorne", 14, WOUNDS, 10, 3, true),
            m_balefire(Weapon::Type::Missile, "White-hot Balefire", 20, 3, 4, 2, -3, RAND_D6),
            m_skullCleaverAxeOfKhorne(Weapon::Type::Melee, "Skull Cleaver Axe of Khorne", 1, 12, 3, 3, -2, 2),
            m_evisceratingClaws(Weapon::Type::Melee, "Eviscerating Claws", 3, RAND_D6, 3, 3, -3, 3),
            m_cavernousJaws(Weapon::Type::Melee, "Cavernous Jaws", 3, 2, 4, 2, -2, 3),
            m_brassPlatedTail(Weapon::Type::Melee, "Brass-plated Tail", 3, RAND_3D6, 4, 4, -1, 3) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, MONSTER,
                      VORGAROTH_THE_SCARRED_AND_SKALOK_THE_SKULL_HOST_OF_KHORNE};
        m_weapons = {&m_balefire, &m_skullCleaverAxeOfKhorne, &m_evisceratingClaws, &m_cavernousJaws,
                     &m_brassPlatedTail};
    }

    bool VorgarothAndSkalok::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_balefire);
        model->addMeleeWeapon(&m_skullCleaverAxeOfKhorne);
        model->addMeleeWeapon(&m_evisceratingClaws);
        model->addMeleeWeapon(&m_cavernousJaws);
        model->addMeleeWeapon(&m_brassPlatedTail);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *VorgarothAndSkalok::Create(const ParameterList &parameters) {
        auto unit = new VorgarothAndSkalok();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VorgarothAndSkalok::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    VorgarothAndSkalok::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    VorgarothAndSkalok::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Vorgaroth the Scarred & Skalok the Skull Host of Khorne",
                                                 factoryMethod);
        }
    }

    void VorgarothAndSkalok::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_evisceratingClaws.setRend(g_damageTable[damageIndex].m_clawsRend);
        m_cavernousJaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_brassPlatedTail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    int VorgarothAndSkalok::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void VorgarothAndSkalok::onRestore() {
        // Restore table-drive attributes
        onWounded();
    }

    Rerolls VorgarothAndSkalok::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Monstrous Trophies
        if (weapon->name() == m_skullCleaverAxeOfKhorne.name()) {
            if (target->hasKeyword(MONSTER)) {
                return RerollFailed;
            }
        }
        return KhorneBase::toWoundRerolls(weapon, target);
    }

    Wounds
    VorgarothAndSkalok::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Monstrous Trophies
        if ((hitRoll == 6) && (weapon->name() == m_skullCleaverAxeOfKhorne.name())) {
            return {weapon->damage(), Dice::rollD3()};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void VorgarothAndSkalok::onCharged() {
        // Crushing Bulk
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::rollD6();
            if (roll >= 3) {
                ip->applyDamage({0, Dice::rollD3()});
            }
        }
        KhorneBase::onCharged();
    }

    Wounds VorgarothAndSkalok::applyWoundSave(const Wounds &wounds) {
        return Unit::applyWoundSave(wounds);
    }

    int VorgarothAndSkalok::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne