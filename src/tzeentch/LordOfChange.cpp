/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/LordOfChange.h"

namespace Tzeentch {
    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 380;

    struct TableEntry {
        int m_move;
        int m_staffToWound;
        int m_infernalGateway;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 1, 3},
                    {10, 2, 4},
                    {9,  3, 4},
                    {8,  4, 4},
                    {7,  5, 5}
            };

    bool LordOfChange::s_registered = false;

    Unit *LordOfChange::Create(const ParameterList &parameters) {
        auto unit = new LordOfChange();
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BalefulSword);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, TzeentchBase::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string LordOfChange::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == RodOfSorcery) return "Rod of Sorcery";
            else if (parameter.intValue == BalefulSword) return "Baleful Sword";
            else if (parameter.intValue == CurvedBeakAndTalons) return "Curved Beak and Talons";
        }
        return TzeentchBase::ValueToString(parameter);
    }

    int LordOfChange::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Rod of Sorcery") return RodOfSorcery;
        else if (enumString == "Baleful Sword") return BalefulSword;
        else if (enumString == "Curved Beak and Talons") return CurvedBeakAndTalons;
        return TzeentchBase::EnumStringToInt(enumString);
    }

    void LordOfChange::Init() {
        if (!s_registered) {
            static const FactoryMethod factoryMethod = {
                    LordOfChange::Create,
                    LordOfChange::ValueToString,
                    LordOfChange::EnumStringToInt,
                    LordOfChange::ComputePoints,
                    {
                            {
                                    ParamType::Enum, "Weapon", LordOfChange::BalefulSword, LordOfChange::BalefulSword,
                                    LordOfChange::CurvedBeakAndTalons, 1
                            },
                            {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None,
                             TzeentchBase::GuildOfSummoners, 1},
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Lord of Change", factoryMethod);
        }
    }

    LordOfChange::LordOfChange() :
            TzeentchBase("Lord of Change", 12, WOUNDS, 10, 4, true),
            m_rodOfSorcery(Weapon::Type::Missile, "Rod of Sorcery", 18, RAND_2D6, 3, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Tzeentch", 3, 4, 3, 1, 0, 2),
            m_sword(Weapon::Type::Melee, "Baleful Sword", 1, 2, 4, 2, -2, 3),
            m_beakAndTalons(Weapon::Type::Melee, "Curved Beak and Wicked Talons", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, MONSTER, HERO, WIZARD, LORD_OF_CHANGE};
        m_weapons = {&m_rodOfSorcery,
                     &m_staff,
                     &m_sword,
                     &m_beakAndTalons};

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool LordOfChange::configure(LordOfChange::WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());
        if (option == RodOfSorcery)
            model->addMissileWeapon(&m_rodOfSorcery);
        else if (option == BalefulSword)
            model->addMeleeWeapon(&m_sword);
        else if (option == CurvedBeakAndTalons)
            model->addMeleeWeapon(&m_beakAndTalons);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void LordOfChange::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void LordOfChange::onWounded() {
        TzeentchBase::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_staff.setToWound(g_damageTable[damageIndex].m_staffToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int LordOfChange::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LordOfChange::rollCasting() const {
        // Mastery of Magic
        auto r0 = Dice::rollD6();
        auto r1 = Dice::rollD6();
        return std::max(r0, r1) * 2 + castingModifier();
    }

    int LordOfChange::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // Tzeentch