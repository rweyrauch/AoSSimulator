/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldbloodCarnosaur.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 230;

    struct TableEntry {
        int m_move;
        int m_forelimbToHit;
        int m_jawsDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 3, 5},
                    {9,  4, 4},
                    {8,  4, 3},
                    {7,  5, 2},
                    {6,  5, 1}
            };

    bool SaurusOldbloodOnCarnosaur::s_registered = false;

    SaurusOldbloodOnCarnosaur::SaurusOldbloodOnCarnosaur() :
            SeraphonBase("Saurus Oldblood on Carnosaur", 10, g_wounds, 8, 4, false),
            m_gauntlet(Weapon::Type::Missile, "Sunbolt Gauntlet", 18, RAND_D6, 3, 4, -1, 1),
            m_spear(Weapon::Type::Melee, "Sunstone Spear", 2, 3, 3, 3, -1, 3),
            m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2, 3, 3, 0, 2),
            m_jaws(Weapon::Type::Melee, "Massive Jaws", 2, 3, 4, 3, -1, 5) {
        m_keywords = {ORDER, SERAPHON, CARNOSAUR, SAURUS, MONSTER, HERO, OLDBLOOD};
        m_weapons = {&m_gauntlet, &m_spear, &m_forelimbs, &m_jaws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_jaws.setMount(true);
        m_forelimbs.setMount(true);
        s_globalBraveryMod.connect(this, &SaurusOldbloodOnCarnosaur::terror, &m_connection);
    }

    SaurusOldbloodOnCarnosaur::~SaurusOldbloodOnCarnosaur() {
        m_connection.disconnect();
    }

    void SaurusOldbloodOnCarnosaur::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_gauntlet);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_forelimbs);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = ComputePoints(1);
    }

    void SaurusOldbloodOnCarnosaur::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *SaurusOldbloodOnCarnosaur::Create(const ParameterList &parameters) {
        auto unit = new SaurusOldbloodOnCarnosaur();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void SaurusOldbloodOnCarnosaur::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SaurusOldbloodOnCarnosaur::Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    SaurusOldbloodOnCarnosaur::ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_saurusCommandTrait[0], g_saurusCommandTrait),
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Oldblood on Carnosaur", factoryMethod);
        }
    }

    void SaurusOldbloodOnCarnosaur::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_jaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
        m_forelimbs.setToHit(g_damageTable[damageIndex].m_forelimbToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t SaurusOldbloodOnCarnosaur::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int SaurusOldbloodOnCarnosaur::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_spear.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusOldbloodOnCarnosaur::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SeraphonBase::toHitModifier(weapon, target);

        // Pinned Down
        if ((weapon->name() == m_jaws.name()) && (target->wounds() >= 7)) {
            mod++;
        }

        return mod;
    }

    int SaurusOldbloodOnCarnosaur::terror(const Unit *target) {
        // Terror
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    int SaurusOldbloodOnCarnosaur::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
