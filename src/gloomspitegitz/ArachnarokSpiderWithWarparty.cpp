/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/ArachnarokSpiderWithWarparty.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz {
    static const int g_basesize = 160;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 220;

    bool ArachnarokSpiderWithSpiderfangWarparty::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_legsAttacks;
        int m_fangsToHit;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 8, 2},
                    {7, 7, 3},
                    {6, 6, 3},
                    {5, 5, 4},
                    {4, 4, 4}
            };

    ArachnarokSpiderWithSpiderfangWarparty::ArachnarokSpiderWithSpiderfangWarparty() :
            GloomspiteGitzBase("Arachnarok Spider with Spiderfang Warparty", 8, g_wounds, 6, 4, true, g_pointsPerUnit),
            m_spiderBows(Weapon::Type::Missile, "Spider-bows", 16, 10, 5, 5, 0, 1),
            m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
            m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3),
            m_crookedSpears(Weapon::Type::Melee, "Crooked Spears", 1, 10, 5, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, MONSTER};
        m_weapons = {&m_spiderBows, &m_chitinousLegs, &m_monstrousFangs, &m_crookedSpears};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_monstrousFangs.setMount(true);
        m_chitinousLegs.setMount(true);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spiderBows);
        model->addMeleeWeapon(&m_chitinousLegs);
        model->addMeleeWeapon(&m_monstrousFangs);
        model->addMeleeWeapon(&m_crookedSpears);
        addModel(model);
    }

    void ArachnarokSpiderWithSpiderfangWarparty::onRestore() {
        GloomspiteGitzBase::onRestore();
        // Reset table-driven attributes
        onWounded();
    }

    void ArachnarokSpiderWithSpiderfangWarparty::onWounded() {
        GloomspiteGitzBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
        m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t ArachnarokSpiderWithSpiderfangWarparty::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *ArachnarokSpiderWithSpiderfangWarparty::Create(const ParameterList &parameters) {
        return new ArachnarokSpiderWithSpiderfangWarparty();
    }

    void ArachnarokSpiderWithSpiderfangWarparty::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArachnarokSpiderWithSpiderfangWarparty::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ArachnarokSpiderWithSpiderfangWarparty::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Arachnarok Spider with Spiderfang Warparty", factoryMethod);
        }
    }

    Wounds ArachnarokSpiderWithSpiderfangWarparty::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                                int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_monstrousFangs.name())) {
            return {0, Dice::RollD3()};
        }
        return GloomspiteGitzBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void ArachnarokSpiderWithSpiderfangWarparty::onCharged() {
        // Voracious Predator
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
        GloomspiteGitzBase::onCharged();
    }

    int ArachnarokSpiderWithSpiderfangWarparty::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace GloomspiteGitz
