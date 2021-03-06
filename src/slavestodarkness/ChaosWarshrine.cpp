/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/ChaosWarshrine.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 170;

    struct TableEntry {
        int m_move;
        int m_fistAttacks;
        int m_protection;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 6, 18},
                    {7, 5, 12},
                    {6, 4, 9},
                    {5, 3, 6},
                    {4, 2, 3}
            };

    bool ChaosWarshrine::s_registered = false;

    Unit *ChaosWarshrine::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);

        return new ChaosWarshrine(legion, mark);
    }

    void ChaosWarshrine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosWarshrine::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosWarshrine::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Warshrine", factoryMethod);
        }
    }

    ChaosWarshrine::ChaosWarshrine(DamnedLegion legion, MarkOfChaos mark) :
            SlavesToDarknessBase(legion, "Chaos Warshrine", 8, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_blade(Weapon::Type::Melee, "Sacrificial Blade", 1, 4, 3, 3, -1, 2),
            m_fists(Weapon::Type::Melee, "Flailing Fists", 1, 6, 4, 3, 0, 2) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, TOTEM, PRIEST, MARK_OF_CHAOS, CHAOS_WARSHRINE};
        m_weapons = {&m_blade, &m_fists};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_fists.setMount(true);

        setMarkOfChaos(mark);

        s_globalWoundSave.connect(this, &ChaosWarshrine::protectionOfTheDarkGods, &m_protectionSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_fists);
        addModel(model);
    }

    ChaosWarshrine::~ChaosWarshrine() {
        m_protectionSlot.disconnect();
    }

    void ChaosWarshrine::onRestore() {
        SlavesToDarknessBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    void ChaosWarshrine::onWounded() {
        SlavesToDarknessBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_fists.setAttacks(g_damageTable[damageIndex].m_fistAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t ChaosWarshrine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ChaosWarshrine::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ChaosWarshrine::onStartHero(PlayerId player) {
        SlavesToDarknessBase::onStartHero(player);

        // Favour of the Ruinous Powers
        if (owningPlayer() == player) {
            Unit *target = nullptr;
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
            for (auto unit : units) {
                if (unit->hasKeyword(MORTAL) && unit->hasKeyword(SLAVES_TO_DARKNESS)) {
                    target = unit;
                    break;
                }
            }
            if (target) {
                auto roll = Dice::RollD6();
                if (roll >= 3) {
                    if (target->hasKeyword(KHORNE)) {
                        // Favour of Khorne
                        target->buffReroll(Attribute::Attacks_Melee, Rerolls::Failed,
                                           {GamePhase::Hero, m_battleRound + 1, player});
                    } else if (target->hasKeyword(TZEENTCH)) {
                        // TODO: Favour of Tzeentch
                        // Allow save against magic
                    } else if (target->hasKeyword(NURGLE)) {
                        // Favour of Nurgle
                        target->buffModifier(Attribute::To_Save_Missile, 1, {GamePhase::Hero, m_battleRound + 1, player});
                        target->buffModifier(Attribute::To_Save_Melee, 1, {GamePhase::Hero, m_battleRound + 1, player});
                    } else if (target->hasKeyword(SLAANESH)) {
                        // Favour of Slanesh
                        target->buffAbility(Ability::Ignore_Battleshock, 1, {GamePhase::Hero, m_battleRound + 1, player});
                    } else if (target->hasKeyword(UNDIVIDED)) {
                        // Favour of Chaos
                        target->buffReroll(Attribute::Charge_Distance, Rerolls::Failed,
                                           {GamePhase::Hero, m_battleRound + 1, player});
                    }
                }
            }
        }
    }

    Wounds ChaosWarshrine::protectionOfTheDarkGods(const Wounds &wounds, const Unit *target, const Unit *attacker) {
        if (isFriendly(target) && target->hasKeyword(SLAVES_TO_DARKNESS) && target->hasKeyword(MORTAL)) {
            auto range = g_damageTable[getDamageTableIndex()].m_protection;
            if (distanceTo(target) < range) {
                int numSixes = 0;
                return ignoreWounds(wounds, 6, numSixes);
            }
        }
        return wounds;
    }

} //namespace SlavesToDarkness
