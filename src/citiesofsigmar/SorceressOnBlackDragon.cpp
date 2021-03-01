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
#include "CoSLore.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_jawsToWound;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
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

        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Witch_Rod);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto drug = (Narcotic)GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        unit->setNarcotic(drug);

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
            static const std::array<int, 2> weapons = {Witch_Rod, Darkling_Sword};
            static FactoryMethod factoryMethod = {
                    SorceressOnBlackDragon::Create,
                    SorceressOnBlackDragon::ValueToString,
                    SorceressOnBlackDragon::EnumStringToInt,
                    SorceressOnBlackDragon::ComputePoints,
                    {
                            EnumParameter("Weapon", Witch_Rod, weapons),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Sorceress on Black Dragon", factoryMethod);
        }
    }

    SorceressOnBlackDragon::SorceressOnBlackDragon() :
            CitizenOfSigmar("Sorceress on Black Dragon", 14, g_wounds, 7, 5, true),
            m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, -7, 0),
            m_rod(Weapon::Type::Melee, "Witch Rod", 1, 1, 4, 3, -1, RAND_D3),
            m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 3, 4, 4, 0, 1),
            m_lash(Weapon::Type::Melee, "Witch Lash", 2, 1, 3, 4, 0, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 1, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, MONSTER, HERO, WIZARD, SORCERESS, DRAGON};
        m_weapons = {&m_noxiousBreath, &m_rod, &m_sword, &m_lash, &m_jaws, &m_claws};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
        m_jaws.setMount(true);
        m_claws.setMount(true);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SorceressOnBlackDragon::configure(WeaponOption option, Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_noxiousBreath);

        if (option == Witch_Rod)
            model->addMeleeWeapon(&m_rod);
        else if (option == Darkling_Sword)
            model->addMeleeWeapon(&m_sword);

        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_claws);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void SorceressOnBlackDragon::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();

        m_bloodSacrificeMod = 0;
    }

    void SorceressOnBlackDragon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    int SorceressOnBlackDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
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
            Dice::RollD6(target->remainingModels(), result);
            return {0, result.rollsGE(6)};
        }
        return CitizenOfSigmar::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SorceressOnBlackDragon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void SorceressOnBlackDragon::onStartHero(PlayerId player) {
        CitizenOfSigmar::onStartHero(player);

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
        auto mod = CitizenOfSigmar::castingModifier();

        mod += m_bloodSacrificeMod;

        return mod;
    }

} // namespace CitiesOfSigmar