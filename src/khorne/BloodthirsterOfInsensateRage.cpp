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
    class BloodthirstyCharge : public CommandAbility {
    public:
        explicit BloodthirstyCharge(Unit *source);

        bool apply(Unit* target, int round) override;
    };

    BloodthirstyCharge::BloodthirstyCharge(Unit *source) :
            CommandAbility(source, "Bloodthirsty Charge", 16, 16, Phase::Charge) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {KHORNE, DAEMON};
    }

    bool BloodthirstyCharge::apply(Unit* target, int round) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(DAEMON) && (unit->hasKeyword(KHORNE))) {
                unit->buffReroll(Charge_Distance, Reroll_Failed, {m_phase, round, m_source->owningPlayer()});
            }
        }
        return true;
    }

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 270;

    struct TableEntry {
        int m_move;
        int m_axeAttacks;
        int m_outrageousCarnage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 5, 4},
                    {9,  5, 3},
                    {8,  4, 2},
                    {7,  4, 1},
                    {6,  3, 1}
            };

    bool BloodthirsterOfInsensateRage::s_registered = false;

    BloodthirsterOfInsensateRage::BloodthirsterOfInsensateRage() :
            KhorneBase("Bloodthirster Of Insensate Rage", 14, g_wounds, 10, 4, true),
            m_greatAxeOfKhorne(Weapon::Type::Melee, "Great Axe of Khorne", 2, 5, 4, 2, -2, RAND_D6) {
        m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO,
                      BLOODTHIRSTER_OF_INSENSATE_RAGE};
        m_weapons = {&m_greatAxeOfKhorne};
        m_battleFieldRole = Leader_Behemoth;
    }

    bool BloodthirsterOfInsensateRage::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_greatAxeOfKhorne);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BloodthirstyCharge>(this));

        m_points = g_pointsPerUnit;

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

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

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
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodthirster Of Insensate Rage", factoryMethod);
        }
    }

    int BloodthirsterOfInsensateRage::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls BloodthirsterOfInsensateRage::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Rage Unbound
        if (m_charged) {
            return Reroll_Ones;
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

            // These mortal wounds are applied to all enemy units within 8".
            // Skip the target unit as the weaponDamage function will handle it.
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
            for (auto ip : units) {
                if (ip != target) {
                    Wounds wounds = {0, Dice::RollSpecial(g_damageTable[damageIndex].m_outrageousCarnage)};
                    ip->applyDamage(wounds, nullptr); // TODO: allow non-const attacker
                }
            }
            return {0, Dice::RollSpecial(g_damageTable[damageIndex].m_outrageousCarnage)};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodthirsterOfInsensateRage::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne