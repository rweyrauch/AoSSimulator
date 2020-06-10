/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostlordThundertusk.h"

namespace OgorMawtribes {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 390;

    struct TableEntry {
        int m_move;
        int m_ice;
        int m_tusksToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 12, 2},
                    {7, 10, 3},
                    {6, 8,  3},
                    {5, 6,  4},
                    {4, 4,  4}
            };

    bool FrostlordOnThundertusk::s_registered = false;

    Unit *FrostlordOnThundertusk::Create(const ParameterList &parameters) {
        auto unit = new FrostlordOnThundertusk();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, None);
        unit->setMawtribe(tribe);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FrostlordOnThundertusk::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int FrostlordOnThundertusk::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void FrostlordOnThundertusk::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostlordOnThundertusk::Create,
                    FrostlordOnThundertusk::ValueToString,
                    FrostlordOnThundertusk::EnumStringToInt,
                    FrostlordOnThundertusk::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frostlord on Thundertusk", factoryMethod);
        }
    }

    FrostlordOnThundertusk::FrostlordOnThundertusk() :
            MawtribesBase("Frostlord on Thundertusk", 8, WOUNDS, 9, 3, false),
            m_ice(Weapon::Type::Missile, "Frost-wreathed Ice", 18, 0, 0, 0, 0, 0),
            m_spear(Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1),
            m_tusks(Weapon::Type::Melee, "Colossal Tusks", 2, 4, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
        m_weapons = {&m_ice, &m_spear, &m_kicks, &m_tusks};
    }

    bool FrostlordOnThundertusk::configure() {
        auto model = new Model(BASESIZE, wounds());

        model->addMissileWeapon(&m_ice);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_tusks);

        addModel(model);

        m_points = FrostlordOnThundertusk::ComputePoints(1);

        return true;
    }

    void FrostlordOnThundertusk::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int FrostlordOnThundertusk::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void FrostlordOnThundertusk::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    int FrostlordOnThundertusk::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        // Numbing Chill
        if (!weapon->isMissile()) mod--;

        return mod;
    }

    void FrostlordOnThundertusk::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (player == owningPlayer()) {
            if (m_meleeTarget) {
                Dice::RollResult result;
                Dice::rollD6(g_damageTable[getDamageTableIndex()].m_ice, result);
                int toWound = 6;
                if (m_meleeTarget->remainingModels() >= 20) toWound -= 2;
                else if (m_meleeTarget->remainingModels() >= 10) toWound -= 1;

                Wounds wounds = {0, result.rollsGE(toWound)};
                m_meleeTarget->applyDamage(wounds);
            }
        }
    }

    int FrostlordOnThundertusk::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace OgorMawtribes
