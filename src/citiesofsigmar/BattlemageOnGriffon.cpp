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
#include "citiesofsigmar/BattlemageOnGriffon.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 90;
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_beaksDamage;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 11, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {15, 3, 6},
                    {13, 2, 5},
                    {11, 2, 4},
                    {9,  1, 3},
                    {7,  1, 2}
            };

    bool BattlemageOnGriffon::s_registered = false;

    Unit *BattlemageOnGriffon::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new BattlemageOnGriffon(city, lore, drug, trait, artefact, general);
    }

    std::string BattlemageOnGriffon::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BattlemageOnGriffon::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BattlemageOnGriffon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BattlemageOnGriffon::Create,
                    BattlemageOnGriffon::ValueToString,
                    BattlemageOnGriffon::EnumStringToInt,
                    BattlemageOnGriffon::ComputePoints,
                    {
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
            s_registered = UnitFactory::Register("Battlemage on Griffon", factoryMethod);
        }
    }

    BattlemageOnGriffon::BattlemageOnGriffon(City city, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Battlemage on Griffon", 15, g_wounds, 6, 5, true, g_pointsPerUnit),
            m_beastStaff(Weapon::Type::Melee, "Beaststaff", 2, 1, 4, 3, -1, RAND_D3),
            m_twinBeaks(Weapon::Type::Melee, "Twin Beaks", 2, 4, 3, 3, -1, 3),
            m_razorClaws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, MONSTER, HERO, WIZARD, BATTLEMAGE};
        m_weapons = {&m_beastStaff, &m_twinBeaks, &m_razorClaws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_razorClaws.setMount(true);
        m_twinBeaks.setMount(true);
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_beastStaff);
        model->addMeleeWeapon(&m_twinBeaks);
        model->addMeleeWeapon(&m_razorClaws);

        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Wildform", 5, 12,
                                                                    std::vector<std::pair<Attribute, int>>{{Attribute::Run_Distance,    2},
                                                                                                           {Attribute::Charge_Distance, 2}},
                                                                    Abilities::Target::SelfAndFriendly));
        m_knownSpells.push_back(std::make_unique<LineOfEffectSpell>(this, "Amber Spear", 7, 18, RAND_D3, 2));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void BattlemageOnGriffon::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void BattlemageOnGriffon::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_twinBeaks.setDamage(g_damageTable[damageIndex].m_beaksDamage);
        m_razorClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t BattlemageOnGriffon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int BattlemageOnGriffon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Two-headed
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_twinBeaks.name())) {
            return 2;
        }
        return CitizenOfSigmar::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int BattlemageOnGriffon::castingModifier() const {
        auto mod = CitizenOfSigmar::castingModifier();

        // Amber Battlemage
        if (Board::Instance()->getRealm() == Realm::Ghur) mod++;

        return mod;
    }

    int BattlemageOnGriffon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar