/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "citiesofsigmar/SorceressOnBlackDragon.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 105;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    struct TableEntry {
        int m_move;
        int m_jawsToWound;
        int m_clawAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 1, 6},
                    {12, 2, 5},
                    {10, 3, 4},
                    {8,  4, 3},
                    {6,  5, 2}
            };


    bool SorceressOnBlackDragon::s_registered = false;

    Unit *SorceressOnBlackDragon::Create(const ParameterList &parameters) {
        auto unit = new SorceressOnBlackDragon();

        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, WitchRod);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(weapon, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SorceressOnBlackDragon::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int SorceressOnBlackDragon::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void SorceressOnBlackDragon::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {WitchRod, DarklingSword};
            static FactoryMethod factoryMethod = {
                    SorceressOnBlackDragon::Create,
                    SorceressOnBlackDragon::ValueToString,
                    SorceressOnBlackDragon::EnumStringToInt,
                    SorceressOnBlackDragon::ComputePoints,
                    {
                            EnumParameter("Weapon", WitchRod, weapons),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Sorceress on Black Dragon", factoryMethod);
        }
    }

    SorceressOnBlackDragon::SorceressOnBlackDragon() :
            CitizenOfSigmar("Sorceress on Black Dragon", 14, WOUNDS, 7, 5, true),
            m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, -7, 0),
            m_rod(Weapon::Type::Melee, "Witch Rod", 1, 1, 4, 3, -1, RAND_D3),
            m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 3, 4, 4, 0, 1),
            m_lash(Weapon::Type::Melee, "Witch Lash", 2, 1, 3, 4, 0, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 1, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, MONSTER, HERO, WIZARD, SORCERESS};
        m_weapons = {&m_noxiousBreath, &m_rod, &m_sword, &m_lash, &m_jaws, &m_claws};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SorceressOnBlackDragon::configure(WeaponOption option, Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_noxiousBreath);

        if (option == WitchRod)
            model->addMeleeWeapon(&m_rod);
        else if (option == DarklingSword)
            model->addMeleeWeapon(&m_sword);

        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_claws);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void SorceressOnBlackDragon::onRestore() {
        // Restore table-driven attributes
        onWounded();

        m_bloodSacrificeMod = 0;
    }

    void SorceressOnBlackDragon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int SorceressOnBlackDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds
    SorceressOnBlackDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Noxious Breath
        if ((weapon->name() == m_noxiousBreath.name())) {
            Dice::RollResult result;
            Dice::rollD6(target->remainingModels(), result);
            return {0, result.rollsGE(6)};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SorceressOnBlackDragon::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void SorceressOnBlackDragon::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Blood Sacrifice
        m_bloodSacrificeMod = 0;
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), DARKLING_COVENS, 3.0);
        if (unit) {
            if (unit->remainingModels() > 1) {
                unit->slay(1);
                m_bloodSacrificeMod = 2;
            }
        }
    }

    int SorceressOnBlackDragon::castingModifier() const {
        auto mod = Unit::castingModifier();

        mod += m_bloodSacrificeMod;

        return mod;
    }

} // namespace CitiesOfSigmar