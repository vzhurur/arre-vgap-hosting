/*
An scoring add-on for phostpdk.
Computes the flags owned by each player and print to stdout two result
tables for flags and another two for hidden flags. 
Flags are configurated by a file, each flag has separates minimum values
for Cols, Mines, Factories or Defense or default values. Defaults values
can be defined also. 
It is possible to use Hidden Flags, that is, flags whose planet id is 
unknown to players. In that case an optional label could be supplied at the end
of either values or default.
Since flags only reads host data it could be executed outside turn hosting.
*/

#include <phostpdk/phostpdk.h>
#include <stdio.h>
#include <string.h>

#define ADDON_CONFIG_FILE "flags.txt"

void flags_addon();

int main(int argc, char** argv)
{
    if (argc > 1)
        gGameDirectory = argv[1];
    if (argc > 2)
        gRootDirectory = argv[2];
    if (argc > 3)
        ErrorExit("usage: %s [gamedir [rootdir]]", argv[0]);

    InitPHOSTLib();
    if (!ReadGlobalData())
        ErrorExit("ReadGlobalData failed");
    if (!ReadHostData())
        ErrorExit("ReadHostData failed");

    flags_addon();

    if (!WriteHostData())
        ErrorExit("WriteHostData failed");
    FreePHOSTLib();
    return 0;
}

Boolean ShowFlags = True;
unsigned DefaultMinCol = 100;
unsigned short DefaultMinFactories = 0, DefaultMinMine = 0, DefaultMinDefenses = 1;

Boolean AssignOption(const char *lhs, char *rhs, const char *lInputLine)
{
    if ( lhs != NULL && rhs != NULL )
    {
        char option[256];
        if ( 1 == sscanf(lhs, "%255s", option) && 0 == strcmp(option, "DefaultStructures") )
        {
            if ( 4 != sscanf(rhs, "%u,%hu,%hu,%hu",
                             &DefaultMinCol, &DefaultMinFactories,
                             &DefaultMinMine, &DefaultMinDefenses) )
            {
                Warning("Bad format to define Default Structures, should be 4 numbers separated by commas");
                return False;
            }
            else
            {
                printf("Default Structures set to: %u,%hu,%hu,%hu\n", DefaultMinCol, DefaultMinFactories,
                             DefaultMinMine, DefaultMinDefenses);
            }
        }
        else
        {
            Warning("Unknown option name, line was: '%s'", lInputLine);
            return False;
        }
    }
    return True;
}

Uns16 flags[RACE_NR+1] = { 0 };

Boolean ComputeFlags(const char *lhs, char *rhs, const char *lInputLine)
{
    unsigned short aPlanet;

    if ( lhs != NULL && rhs != NULL  )
    {
        if (  (1 == sscanf(lhs, "%hu", &aPlanet)) && IsPlanetExist( aPlanet ) )
        {
            unsigned MinCol = DefaultMinCol;
            unsigned short MinFactories = DefaultMinFactories,
                  MinMine = DefaultMinMine, MinDefenses = DefaultMinDefenses;
            char showId[256] = "ShowId";
            char value[256];
            int goal;

            if ( ! ShowFlags ) strcpy(showId, "Hidden");

            if ( 2 == sscanf(rhs, " %255[^,], %255[^\n]", value, showId) && 0 == strcmp(value, "Default") )
            {
            } 
            else if ( 1 == sscanf(rhs, " %255s", value) && 0 == strcmp(value, "Default") )
            {
            } 
            else if ( 5 == sscanf(rhs, "%u,%hu,%hu,%hu, %255[^\n]",&MinCol, &MinFactories, &MinMine, &MinDefenses, showId) )
            {
            }
            else if ( 4 == sscanf(rhs, "%u,%hu,%hu,%hu",&MinCol, &MinFactories, &MinMine, &MinDefenses) )
            {
            }
            else
            {
                Error("Unknown target structures for flag, should be either 'Default' or 4 numbers, optionally followed by a label to hide planet id");
                return False;
            }

            goal = PlanetCargo(aPlanet, COLONISTS) >= MinCol &&
                   PlanetFactories(aPlanet) >= MinFactories &&
                   PlanetMines(aPlanet) >= MinMine &&
                   PlanetDefense(aPlanet) >= MinDefenses;

            flags[PlanetOwner(aPlanet)] += goal;

            if ( 0 == strcmp(showId, "ShowId") ) 
            {
                sprintf(showId, "%10u", aPlanet);
            }
            
            if ( goal )
            {
                printf("%s %10u \n", showId, PlanetOwner(aPlanet));
                /*printf("%10u %10u %10u %10u %10u %10u\n", aPlanet, PlanetOwner(aPlanet),
                   PlanetCargo(aPlanet, COLONISTS),
                   PlanetFactories(aPlanet),
                   PlanetMines(aPlanet),
                   PlanetDefense(aPlanet));*/
            }
            else
            {
                printf("%s %10s\n", showId, PlanetOwner(aPlanet) == 0 ? "Unowned" : "Not enough");
            }
        }
        else
        {
            Error("Planet not exist for flag in '%s'", lInputLine );
            return False;
        }
    }
    return True;
}

void print_header()
{
    printf("\n%s\n"
           "==============\n"
           "\n"
           " %10s %10s\n " /* "%10s %10s %10s %10s\n" */
           " ---------- ---------- \n", /* " ---------- ---------- ---------- ----------\n",*/
           ShowFlags ? "Flag Report" : "Hidden Flag Report", 
           "Planet", "Player" /*, "Colonist", "Factories", "Mines", "Defenses"*/);    
}

void print_player()
{
    int lPlayer;
    printf("\n%s\n"
               "========================\n"
               "\n"
               " Player    Flags\n"
               "--------  ------- \n",
               ShowFlags ? "Flag Report by Player" : "Hidden Flag Report by Player"
               );
               
    for ( lPlayer = 1; lPlayer <= RACE_NR; ++lPlayer)
    {
        if (PlayerIsActive(lPlayer))
        {
            printf("%4d      %4d\n", lPlayer, flags[lPlayer]);
        }
    }
}

void flags_addon()
{
    const char* config_file_name = ADDON_CONFIG_FILE;
    int lPlayer;

    FILE *f = OpenInputFile(config_file_name, GAME_DIR_ONLY | TEXT_MODE);

    if ( f != NULL )
    {
        ConfigFileReaderEx( f, config_file_name, "OPTION_FLAGS", True, AssignOption, Error, True );
        fclose(f);

        ShowFlags = True;
        print_header();

        f = OpenInputFile(config_file_name, GAME_DIR_ONLY | TEXT_MODE);
        ConfigFileReaderEx( f, config_file_name, "FLAGS", False, ComputeFlags, Error, False );

        print_player();

        fclose(f);
        
        for ( lPlayer = 1; lPlayer <= RACE_NR; ++lPlayer)
        {
            flags[lPlayer] = 0;
        }        

        ShowFlags = False;
        print_header();
        
        f = OpenInputFile(config_file_name, GAME_DIR_ONLY | TEXT_MODE);
        ConfigFileReaderEx( f, config_file_name, "HIDDEN_FLAGS", False, ComputeFlags, Error, False );

        print_player();

        fclose(f);
    }
    else
    {
        Error("Could not find configuration file for Flags Add-on: '%s'", config_file_name);
    }
}
