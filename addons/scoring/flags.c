/*
An scoring add-on for phostpdk.
Computes the flags owned by each player and produces two results files
(by flag or by player) to be display in a Web. 
Flags are configurated by a file, each flag has separates minimum values
for Cols, Mines, Factories or Defense or default values. Defaults values
can be defined also. 
Since flags only reads host data it could be executed outside turn hosting.
*/

#include <phost_user/phostpdk/phostpdk.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_FILE "flags.txt"
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
Uns32 DefaultMinCol = 100;
Uns16 DefaultMinFactories = 0, DefaultMinMine = 0, DefaultMinDefenses = 1;

Boolean AssignOption(const char *lhs, char *rhs, const char *lInputLine)
{
    if ( lhs != NULL && rhs != NULL )
    {
        char option[256], value[256];
        if ( 1 == sscanf(lhs, "%255s", option) && 0 == strcmp(lhs, "ShowFlags") )
        {
            ShowFlags = 1 == sscanf(rhs, "%255s", value) && 0 == strcmp(rhs, "Yes");
        }
        else if ( 1 == sscanf(lhs, "%255s", option) && 0 == strcmp(lhs, "DefaultStructures") )
        {
            if ( 4 != sscanf(rhs, "%u,%hu,%hu,%hu",
                             &DefaultMinCol, &DefaultMinFactories,
                             &DefaultMinMine, &DefaultMinDefenses) )
            {
                Warning("Bad format to define Default Structures, should be 4 numbers separated by commas");
                return False;
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
    Uns16 aPlanet;

    if ( lhs != NULL && rhs != NULL && 1 == sscanf(lhs, "%hu", &aPlanet) )
    {
        if ( IsPlanetExist( aPlanet ) )
        {
            Uns32 MinCol = DefaultMinCol;
            Uns16 MinFactories = DefaultMinFactories,
                  MinMine = DefaultMinMine, MinDefenses = DefaultMinDefenses;
            char value[256];
            int goal;

            if ( 1 == sscanf(rhs, "%255s", value) && 0 == strcmp(value, "Default") )
            {
            }
            else if ( 4 == sscanf(rhs, "%u,%hu,%hu,%hu",
                             &MinCol, &MinFactories, &MinMine, &MinDefenses) )
            {
            }
            else
            {
                Error("Unknown target structures for flag, should be either 'Default' or 4 numbers");
                return False;
            }

            goal = PlanetCargo(aPlanet, COLONISTS) >= MinCol &&
                   PlanetFactories(aPlanet) >= MinFactories &&
                   PlanetMines(aPlanet) >= MinMine &&
                   PlanetDefense(aPlanet) >= MinDefenses;

            flags[PlanetOwner(aPlanet)] += goal;

            if ( goal && ShowFlags == True )
            {
                printf("%10u %10u %10u %10u %10u %10u\n", aPlanet, PlanetOwner(aPlanet),
                   PlanetCargo(aPlanet, COLONISTS),
                   PlanetFactories(aPlanet),
                   PlanetMines(aPlanet),
                   PlanetDefense(aPlanet));
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

void flags_addon()
{
    const char* config_file_name = CONFIG_FILE;
    int lPlayer;

    FILE *f = OpenInputFile(config_file_name, GAME_DIR_ONLY | TEXT_MODE);

    if ( f != NULL )
    {
        ConfigFileReaderEx( f, config_file_name, "OPTION_FLAGS", True, AssignOption, Error, True );
        fclose(f);

        if ( ShowFlags == True )
        {
            printf("\nFlag Report\n"
                   "==============\n"
                   "\n"
                   "%10s %10s %10s %10s %10s\n"
                   " ---------- ---------- ---------- ---------- ---------- ----------\n",
                   "Planet", "Player", "Colonist" "Factories", "Mines", "Defenses");
        }

        f = OpenInputFile(config_file_name, GAME_DIR_ONLY | TEXT_MODE);
        ConfigFileReaderEx( f, config_file_name, "Flags", False, ComputeFlags, Error, False );

        printf("\nFlags Report by Player\n"
               "========================\n"
               "\n"
               " Player    Flags\n"
               "--------  ------- \n");
        for ( lPlayer = 1; lPlayer <= RACE_NR; ++lPlayer)
        {
            if (PlayerIsActive(lPlayer))
            {
                printf("%4d      %4d\n", lPlayer, flags[lPlayer]);
            }
        }

        fclose(f);
    }
    else
    {
        Error("Could not find configuration file for Flags Add-on: '%s'", config_file_name);
    }
}
