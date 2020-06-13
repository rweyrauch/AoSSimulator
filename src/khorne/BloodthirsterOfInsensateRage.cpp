/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodthirsterOfInsensateRage.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 270;

    struct TableEntry {
        int m_move;
        int m_axeAttacks;
        int m_outrageousCarnage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {10, 5, 4},
                    {9,  5, 3},
                    {8,  4, 2},
                    {7,  4, 1},
                    {6,  3, 1}
            };

    bool BloodthirsterOfInsensateRage::s_registered = false;

    BloodthirsterOfInsensateRage::BloodthirsterOfInsensateRage() :
            KhorneBase("Bloodthirster Of Insensate Rage", 14, WOUNDS, 10, 4, true),
            m_greatAxeOfKhorne(Weapon::Type::Melee, "Great Axe of Khorne", 2, 5, 4, 2, -2, RAND_D6) {
        m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO,
                      BLOODTHIRSTER_OF_INSENSATE_RAGE};
        m_weapons = {&m_greatAxeOfKhorne};
    }

    bool BloodthirsterOfInsensateRage::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_greatAxeOfKhorne);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *BloodthirsterOfInsensateRage::Create(const ParameterList &parameters) {
        auto unit = new BloodthirsterOfInsensateRage();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodthirsterOfInsensateRage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BloodthirsterOfInsensateRage::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    BloodthirsterOfInsensateRage::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodthirster Of Insensate Rage", factoryMethod);
        }
    }

    int BloodthirsterOfInsensateRage::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls BloodthirsterOfInsensateRage::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Rage Unbound
        if (m_charged) {
            return RerollOnes;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    void BloodthirsterOfInsensateRage::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_greatAxeOfKhorne.setAttacks(g_damageTable[damageIndex].m_axeAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }


    void BloodthirsterOfInsensateRage::onRestore() {
        // Restore table-drive attributes
        onWounded();
    }

    Wounds BloodthirsterOfInsensateRage::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                      int woundRoll) const {
        // Outrageous Carnage
        if ((woundRoll == 6) && weapon->name() == m_greatAxeOfKhorne.name()) {
            const int damageIndex = getDamageTableIndex();

            Wounds wounds = {0, Dice::rollSpecial(g_damageTable[damageIndex].m_outrageousCarnage)};

            // These mortal wounds are applied to all enemy units within 8".
            // Skip the target unit as the weaponDamage function will handle it.
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
            for (auto ip : units) {
                if (ip != target) {
                    // TODO: need to allow unit a wound save
                    Wounds actualWounds = wounds; //ip->applyWoundSave(wounds);
                    ip->applyDamage(actualWounds);
                }
            }
            return wounds;
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodthirsterOfInsensateRage::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne