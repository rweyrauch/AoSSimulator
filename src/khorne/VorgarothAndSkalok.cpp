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
    static const int g_basesize = 280; // x210 oval
    static const int g_wounds = 30;
    static const int g_pointsPerUnit = 1100;

    struct TableEntry {
        int m_move;
        int m_clawsRend;
        int m_jawsToWound;
        int m_tailAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {5, 10, 15, 20, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, -3, 2, RAND_3D6},
                    {11, -3, 3, RAND_2D6},
                    {10, -2, 4, RAND_D6},
                    {9,  -1, 5, RAND_D3},
                    {8,  -1, 6, 1}
            };

    bool VorgarothAndSkalok::s_registered = false;

    VorgarothAndSkalok::VorgarothAndSkalok() :
            KhorneBase("Vorgaroth the Scarred & Skalok the Skull Host of Khorne", 14, g_wounds, 10, 3, true),
            m_balefire(Weapon::Type::Missile, "White-hot Balefire", 20, 3, 4, 2, -3, RAND_D6),
            m_skullCleaverAxeOfKhorne(Weapon::Type::Melee, "Skull Cleaver Axe of Khorne", 1, 12, 3, 3, -2, 2),
            m_evisceratingClaws(Weapon::Type::Melee, "Eviscerating Claws", 3, RAND_D6, 3, 3, -3, 3),
            m_cavernousJaws(Weapon::Type::Melee, "Cavernous Jaws", 3, 2, 4, 2, -2, 3),
            m_brassPlatedTail(Weapon::Type::Melee, "Brass-plated Tail", 3, RAND_3D6, 4, 4, -1, 3) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, MONSTER,
                      VORGAROTH_THE_SCARRED_AND_SKALOK_THE_SKULL_HOST_OF_KHORNE};
        m_weapons = {&m_balefire, &m_skullCleaverAxeOfKhorne, &m_evisceratingClaws, &m_cavernousJaws,
                     &m_brassPlatedTail};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
        m_evisceratingClaws.setMount(true);
        m_cavernousJaws.setMount(true);
        m_brassPlatedTail.setMount(true);

        s_globalCastMod.connect(this, &VorgarothAndSkalok::wingsOfFury, &m_wingOfFuryConnection);
        s_globalUnbindMod.connect(this, &VorgarothAndSkalok::wingsOfFury, &m_wingOfFuryConnection);
    }

    VorgarothAndSkalok::~VorgarothAndSkalok() {
        m_wingOfFuryConnection.disconnect();
    }

    bool VorgarothAndSkalok::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_balefire);
        model->addMeleeWeapon(&m_skullCleaverAxeOfKhorne);
        model->addMeleeWeapon(&m_evisceratingClaws);
        model->addMeleeWeapon(&m_cavernousJaws);
        model->addMeleeWeapon(&m_brassPlatedTail);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *VorgarothAndSkalok::Create(const ParameterList &parameters) {
        auto unit = new VorgarothAndSkalok();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

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
                            BoolParameter("General")
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
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void VorgarothAndSkalok::onRestore() {
        // Restore table-drive attributes
        onWounded();
        KhorneBase::onRestore();
    }

    Rerolls VorgarothAndSkalok::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Monstrous Trophies
        if (weapon->name() == m_skullCleaverAxeOfKhorne.name()) {
            if (target->hasKeyword(MONSTER)) {
                return Reroll_Failed;
            }
        }
        return KhorneBase::toWoundRerolls(weapon, target);
    }

    Wounds
    VorgarothAndSkalok::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Monstrous Trophies
        if ((hitRoll == 6) && (weapon->name() == m_skullCleaverAxeOfKhorne.name())) {
            return {weapon->damage(), Dice::RollD3()};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void VorgarothAndSkalok::onCharged() {
        // Crushing Bulk
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 3) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
        KhorneBase::onCharged();
    }

    Wounds VorgarothAndSkalok::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    int VorgarothAndSkalok::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int VorgarothAndSkalok::wingsOfFury(const Unit *caster) {
        if (!isFriendly(caster) && (distanceTo(caster) <= 18.0)) {
            return -3;
        }
        return 0;
    }

    void VorgarothAndSkalok::onStartHero(PlayerId player) {
        KhorneBase::onStartHero(player);

        // Fuelled by Death
        if (owningPlayer() == player) {
            if (remainingWounds() < wounds()) {
                auto unit = Board::Instance()->getNearestUnit(this, owningPlayer());
                if (unit && (distanceTo(unit) < 3.0)) {
                    if (Dice::RollD6() >= unit->wounds()) {
                        unit->slay(1);
                        heal(unit->wounds());
                    }
                }
            }
        }
    }

} // namespace Khorne