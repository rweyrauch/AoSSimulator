#include <gtkmm.h>
#include <map>
#include <string>
#include <Board.h>
#include <Dice.h>
#include <ManoAMano.h>
#include <UnitFactory.h>
#include <iostream>

static std::map<std::string, int> g_allianceNameLookup = {
    { "Order", ORDER },
    { "Chaos", CHAOS },
    { "Death", DEATH },
    { "Destruction", DESTRUCTION }
};

static std::map<std::string, int> g_factionNameLookup = {
    { "Stormcast Eternal", STORMCAST_ETERNAL, },
    { "Khorne", KHORNE },
    { "Sylvaneth", SYLVANETH },
    { "Gloomspite Gitz", GLOOMSPITE_GITZ },
    { "Moonclan", MOONCLAN },
    { "Nighthaunt", NIGHTHAUNT },
    { "Daughters of Khaine", DAUGHTERS_OF_KHAINE },
    { "Idoneth Deepkin", IDONETH_DEEPKIN },
    { "Beasts of Chaos", BEASTS_OF_CHAOS },
    { "Slaanesh", SLAANESH },
    { "Tzeentch", TZEENTCH },
    { "Nurgle", NURGLE },
    { "Slaves to Darkness", SLAVES_TO_DARKNESS },
    { "Flesh Eaters Court", FLESH_EATERS_COURT },
    { "Grand Host of Nagash", GRAND_HOST_OF_NAGASH },
    { "Legion of Blood", LEGION_OF_BLOOD },
    { "Legion of Night", LEGION_OF_NIGHT },
    { "Legion of Sacrament", LEGION_OF_SACRAMENT },
    { "Soulblight", SOULBLIGHT },
    { "Beastclaw Raiders", BEASTCLAW_RAIDERS },
    { "Bonesplitterz", BONESPLITTERZ },
    { "Greenskinz", GREENSKINZ },
    { "Ironjawz", IRONJAWZ },
    { "Darkling Covens", DARKLING_COVENS },
    { "Devoted of Sigmar", DEVOTED_OF_SIGMAR },
    { "Dispossessed", DISPOSSESSED },
    { "Eldritch Council", ELDRITCH_COUNCIL },
    { "Free Peoples", FREE_PEOPLES },
    { "Fyreslayers", FYRESLAYERS },
    { "Kharadron Overlords", KHARADRON_OVERLORDS },
    { "Order Draconis", ORDER_DRACONIS },
    { "Order Serpentis", ORDER_SERPENTIS },
    { "Phoenix Temple", PHOENIX_TEMPLE },
    { "Scourge Privateers", SCOURGE_PRIVATEERS },
    { "Seraphon", SERAPHON },
    { "Shadowblades", SHADOWBLADES },
    { "Swifthawk Agents", SWIFTHAWK_AGENTS },
    { "Wanderers", WANDERER },
    { "Skaven", SKAVEN },
    { "Deathrattle", DEATHRATTLE },
    { "Deadwalkers", DEADWALKERS},
};

static std::string reverseLookup(int keyword)
{
    for (auto ip : g_factionNameLookup)
    {
        if (ip.second == keyword)
            return ip.first;
    }
    std::cerr << "Keyword not found in Faction Name Table " << keyword << std::endl;
    return "Not Found";
}

static Gtk::Window* pWindow = nullptr;
static Gtk::ComboBoxText *pRedAlliance = nullptr;
static Gtk::ComboBoxText *pBlueAlliance = nullptr;
static Gtk::ComboBoxText *pRedFaction = nullptr;
static Gtk::ComboBoxText *pBlueFaction = nullptr;
static Gtk::ComboBoxText *pRedUnits = nullptr;
static Gtk::ComboBoxText *pBlueUnits = nullptr;
static Gtk::Box *pRedUnitConfig = nullptr;
static Gtk::Grid *pRedUnitGrid = nullptr;
static Gtk::Box *pBlueUnitConfig = nullptr;
static Gtk::Grid* pBlueUnitGrid = nullptr;

static int g_numRounds = 5;
static int g_verboseLevel = 0; // Verbosity::Silence == 0
static int g_numIterations = 1;
static bool g_saveMaps = false;
static int g_redAlliance = ORDER;
static int g_blueAlliance = ORDER;

static ManoAMano* g_battle = nullptr;
static Unit* g_pRed = nullptr;
static Unit* g_pBlue = nullptr;

static void runSimulation();

void createConfigUI(const std::string& unitName, Gtk::Box *pContainer, Gtk::Grid* pGrid);

static void on_quit_clicked()
{
    if (pWindow)
        pWindow->hide();
}

static void on_start_clicked()
{
    runSimulation();
}

static void populateUnits(const std::string& factionName, Gtk::ComboBoxText *pCombo)
{
    pCombo->remove_all();
    std::list<std::string> unitNames;

    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        auto ki = g_factionNameLookup.find(factionName);
        if (ki != g_factionNameLookup.end())
        {
            // filter based on keyword
            if (ki->second == ruip->second.m_faction)
                unitNames.push_back(ruip->first);
        }
    }
    if (!unitNames.empty())
    {
        unitNames.sort();
        unitNames.unique();
        for (const auto &ip : unitNames)
        {
            pCombo->append(ip);
        }
        pCombo->set_active(0);
    }
}

static void populateFactions(int allianceId, Gtk::ComboBoxText *pCombo)
{
    pCombo->remove_all();
    std::list<int> factionKeywords;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        if (allianceId == ruip->second.m_grandAlliance)
        {
            factionKeywords.push_back(ruip->second.m_faction);
        }
    }
    factionKeywords.sort();
    factionKeywords.unique();
    for (auto ip : factionKeywords)
    {
        pCombo->append(reverseLookup(ip));
    }
    pCombo->set_active(0);
}

static void on_red_alliance_selected()
{
    if (pRedAlliance)
    {
        auto alliance = pRedAlliance->get_active_text();
        g_redAlliance = g_allianceNameLookup[alliance];

        std::cout << "Red selected Grand Alliance " << alliance << " with ID: " << g_redAlliance << std::endl;

        populateFactions(g_redAlliance, pRedFaction);
    }
}

static void on_blue_alliance_selected()
{
    if (pBlueAlliance)
    {
        auto alliance = pBlueAlliance->get_active_text();
        g_blueAlliance = g_allianceNameLookup[alliance];

        std::cout << "Blue selected Grand Alliance " << alliance << " with ID: " << g_blueAlliance << std::endl;

        populateFactions(g_blueAlliance, pBlueFaction);
    }
}

static void on_red_faction_selected()
{
    if (pRedFaction && pRedUnits)
    {
        auto factionName = pRedFaction->get_active_text();
        populateUnits(factionName, pRedUnits);
    }
}

static void on_blue_faction_selected()
{
    if (pBlueFaction && pBlueUnits)
    {
        auto factionName = pBlueFaction->get_active_text();
        populateUnits(factionName, pBlueUnits);
    }
}

static void on_red_unit_select()
{
    if (pRedUnits && pRedUnitConfig)
    {
        auto unitName = pRedUnits->get_active_text();
        createConfigUI(unitName, pRedUnitConfig, pRedUnitGrid);
    }
}

static void on_blue_unit_select()
{
    if (pBlueUnits && pBlueUnitConfig)
    {
        auto unitName = pBlueUnits->get_active_text();
        createConfigUI(unitName, pBlueUnitConfig, pBlueUnitGrid);
    }
}

static void runSimulation()
{
    if (g_pRed == nullptr || g_pBlue == nullptr)
    {
        std::cerr << "Player units not created." << std::endl;
        return;
    }

    std::string mapBaseName("battlemap");

    auto board = Board::Instance();

    g_battle->combatants(g_pRed, g_pBlue);

    std::cout << "Red Points: " << g_pRed->points() << "   Blue Points: " << g_pBlue->points() << std::endl;

    std::stringstream fn;

    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    for (auto i = 0; i < g_numIterations; i++)
    {
        g_pRed->restore();
        g_pBlue->restore();

        g_battle->start();

        if (g_saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_start.png";
            board->render(fn.str());
        }

        while (!g_battle->done())
        {
            g_battle->simulate();

            auto round = g_battle->currentRound();

            if (g_saveMaps)
            {
                fn.str("");
                fn << mapBaseName << "_round_" << round << ".png";
                board->render(fn.str());
            }

            g_battle->next();
        }

        auto victor = g_battle->getVictor();

        if (g_verboseLevel > 0)
        {
            std::cout << "Team " << PlayerIdToString(victor) << " was victorious." << std::endl;
            g_battle->logStatistics();
        }

        if (victor == PlayerId::Blue)
            blueVictories++;
        else if (victor == PlayerId::Red)
            redVictories++;
        else
            ties++;

        if (g_saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_end.png";
            board->render(fn.str());
        }
    }

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << (float)redVictories * 100.0f/g_numIterations << std::endl
              << "\tBlue: " << (float)blueVictories * 100.0f/g_numIterations << std::endl
              << "\tTies: " << (float)ties * 100.0f/g_numIterations << std::endl;
}

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    Initialize(Verbosity::Normal);

    g_battle = new ManoAMano(g_numRounds);

    // Construct the top-level UI from the glade description.
    auto builder = Gtk::Builder::create_from_file("manoamano.glade");

    // Find the  top-level window.
    builder->get_widget("manoamanoWindow", pWindow);

    // Hook up callbacks
    Gtk::Button* pQuitButton = nullptr;
    builder->get_widget("quitButton", pQuitButton);
    pQuitButton->signal_clicked().connect(sigc::ptr_fun(on_quit_clicked));

    Gtk::Button* pStartButton = nullptr;
    builder->get_widget("startButton", pStartButton);
    pStartButton->signal_clicked().connect(sigc::ptr_fun(on_start_clicked));

    builder->get_widget("redGrandAlliance", pRedAlliance);
    builder->get_widget("blueGrandAlliance", pBlueAlliance);

    pRedAlliance->signal_changed().connect(sigc::ptr_fun(on_red_alliance_selected));
    pBlueAlliance->signal_changed().connect(sigc::ptr_fun(on_blue_alliance_selected));

    pRedAlliance->set_active(0);
    pBlueAlliance->set_active(0);

    // Add factions
    builder->get_widget("redFaction", pRedFaction);
    builder->get_widget("blueFaction", pBlueFaction);

    pRedFaction->signal_changed().connect(sigc::ptr_fun(on_red_faction_selected));
    pBlueFaction->signal_changed().connect(sigc::ptr_fun(on_blue_faction_selected));

    populateFactions(0, pRedFaction);
    pRedFaction->set_active(0);
    populateFactions(0, pBlueFaction);
    pBlueFaction->set_active(0);

    builder->get_widget("redUnits", pRedUnits);
    builder->get_widget("blueUnits", pBlueUnits);

    pRedUnits->signal_changed().connect(sigc::ptr_fun(on_red_unit_select));
    pBlueUnits->signal_changed().connect(sigc::ptr_fun(on_blue_unit_select));

    auto factionName = pRedFaction->get_active_text();
    if (!factionName.empty())
        populateUnits(factionName, pRedUnits);

    factionName = pBlueFaction->get_active_text();
    if (!factionName.empty())
        populateUnits(factionName, pBlueUnits);

    builder->get_widget("redUnitConfig", pRedUnitConfig);
    builder->get_widget("blueUnitConfig", pBlueUnitConfig);

    createConfigUI(pRedUnits->get_active_text(), pRedUnitConfig, pRedUnitGrid);
    createConfigUI(pBlueUnits->get_active_text(), pBlueUnitConfig, pBlueUnitGrid);

    return app->run(*pWindow);
}

void createConfigUI(const std::string& unitName, Gtk::Box *pContainer, Gtk::Grid* pGrid)
{
    auto factory = UnitFactory::LookupUnit(unitName);
    if (factory)
    {
        if (pGrid)
        {
            pContainer->remove(*pGrid);
        }

        pGrid = Gtk::manage(new Gtk::Grid());
        pGrid->set_row_spacing(4);
        pGrid->set_column_spacing(4);

        int left = 0;
        int right = 1;
        int top = 0;
        std::vector<Parameter> defaultParams = factory->m_parameters;
        for (const auto& ip : defaultParams)
        {
            if (ip.m_paramType == ParamType::Integer)
            {
                Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
                pLabel->set_text(ip.m_name);
                pGrid->attach(*pLabel, left, top, 1, 1);
                pLabel->show();

                //if (factory->m_paramToString == nullptr)
                {
                    Gtk::Entry *pEntry = Gtk::manage(new Gtk::Entry());
                    pEntry->set_text(std::to_string(ip.m_intValue));
                    pGrid->attach(*pEntry, right, top, 1, 1);
                    pEntry->show();
                }
                /*
                else
                {
                    Gtk::ComboBoxText *pCombo = Gtk::manage(new Gtk::ComboBoxText());
                    for (auto i = ip.m_minValue; i <= ip.m_maxValue; i += ip.m_increment)
                    {
                        Parameter param = ip;
                        param.m_intValue = i;
                        pCombo->append(factory->m_paramToString(param));
                    }
                }
                */
                top++;
            }
            else if (ip.m_paramType == ParamType::Boolean)
            {
                Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
                pLabel->set_text(ip.m_name);
                pGrid->attach(*pLabel, left, top, 1, 1);
                pLabel->show();
                Gtk::CheckButton* pCheck = Gtk::manage(new Gtk::CheckButton());
                pCheck->set_active(ip.m_boolValue);
                pGrid->attach(*pCheck, right, top, 1, 1);
                pCheck->show();
                top++;
            }
        }
        pContainer->pack_start(*pGrid);
        pGrid->show();
    }
}
