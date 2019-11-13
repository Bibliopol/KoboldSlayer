//this is an adventure game by J.E. Hogue.
//you play as a kobold extermination specialist; nicknamed the "Kobold Slayer."
//this game is turn based, wherein you get an idle phase and then an attack phase.
//in the idle phase you can execute any one action excluding directly attacking an enemy.
//in the attack phase you select one target to attack, while the kobolds will ALL attack you simultaneously.
//depending on the circumstances, the player can also inspect and interact with in-game objects.
//the player is represented as an asterisk, while enemies are represented by letters. There is 'k' for Kobold, 'g' for Giant Kobold, and 's' for Shaman.
//objects of interest will be represented by various icons. Discover them all!

#include <vector>		//for vectors.
#include <iostream>		//for IO.
#include <algorithm>	//for algorithms.
#include <cstdlib>		//for random numbers.
#include <ctime>		//for retrieving current time.

using namespace std; //for ease of typing.

//function prototypes.
void getAnswer(string & myAnswer); //gets an answer from the player.
void checkHealth(int health, const int & maxHealth, int armorDurability); //prints current health in health/maxHealth format.
void printLevel(vector < vector < string >> grid); //prints the current level.
vector < vector < string >> & getLevel(int counter, vector < vector < string >> & level1, vector < vector < string >> & level2, vector < vector < string >> & level3, vector < vector < string >> & level3Reveal, vector < vector < string >> & level4, vector < vector < string >> & level5, vector < vector < string >> & level6, vector < vector < string >> & level7); //gets the current level based on the 'counter' variable in the main() loop.
vector < int > findObjPos(vector < vector < string >> & grid, string entered); //finds and returns position of nearest entered object
void equip(string answer, vector < string > & backpack, vector < string > & equipped); //command for equipping an item.
void inspect(string answer, vector < vector < string >> currentLevel, int level, const string level1Desc, const string level2Desc, const string level3Desc, const string level4Desc, const string level5Desc, const string level6Desc, const string level7Desc, vector < string > & backpack, bool & inspO, bool & inspKobold, bool & denyAttack, bool & denyEnemAttack); //command for inspecting an object.
void use(string & answer, int & health, int maxHealth, bool & holyLight, int & miraclesLeft, int & knives, int knifeDamage, bool & denyAttack, int & berserkCharges, bool & bActive, vector < string > & backpack, vector < string > equipped, bool & gateScrollCity, bool & gateScrollOcean, bool & poisoned, bool & attackTwo, bool & onFire, bool & confused, bool & targeActive, vector < vector < string >> currentLevel, vector < string > enemyType, int armorDurability, vector < int > defaultDamage); //command for using items or abilities.
void inventory(vector < string > backpack, vector < string > equipped, int miraclesLeft, int knives, int berserkCharges, int level); //command for printing current inventory.
int dmgCalc(vector < vector < string >> grid, const vector < int > defaultDamage, const vector < string > enemyType); //takes the number of enemies of each type in the level and calculates total damage figure.
vector < int > enemCalc(vector < vector < string >> grid, const vector < string > enemyType); //calculates the number of enemies in a level.
void killEnem(vector < vector < string >> & grid, string enemyType, int numEnemies, const vector < string > enemyName); //kills a certain number of the selected enemy.
bool removeDead(int currentEnemy, vector<int>& variableSingleHealth, vector<int> variableAllHealth, vector<vector<string>>& currentLevel, const vector<string> enemyType, const vector<string> enemyName); //removes dead enemies from the map.
void options(); //prints options
void introAndInstruct(const int poisonChance, vector<string> equipped, const int fireDamage, const int knifeDamage); //prints and introduction and instructions
void idlePhase(const int& maxHealth, int level, const string level1Desc, const string level2Desc, const string level3Desc, const string level4Desc, const string level5Desc, const string level6Desc, const string level7Desc, vector<string>& backpack, bool& inspO, bool& inspKobold, bool& denyAttack, bool& denyEnemAttack, string& answer, int& health, bool& holyLight, int& miraclesLeft, int& knives, int knifeDamage, int& berserkCharges, bool& bActive, bool& gateScrollCity, bool& gateScrollOcean, bool& poisoned, bool& attackTwo, bool& onFire, bool& confused, bool& targeActive, vector<vector<string>> currentLevel, vector<string> enemyType, int armorDurability, vector<int> defaultDamage, vector<string>& equipped); //idle phase main function

int main()
{
      /*backpack explanation:
     gateScroll allows the player to write a location* and have a portal open connecting the current location and there for a moment.
     *to clarify, one can choose the bottom of the ocean, or the nearby city to escape. A cowardly move. (Ends the game early with no points.)
     antidote allows the player to dispel poison effects from themself or others.
     healthPot is a health potion that heals the player up to max health.
     stamPot is a stamina potion that allows the player to attack for double damage on one target; as well as take 50% less damage.
     fireWater allows the player to set every enemy in the level on fire, doing 2 damage per turn. Must have torch in backpack to use.
     smokeBomb confuses all goblins in a current level, denying them from attacking for one turn.
     targe is a sharp-rimmed shield that can be used as a weapon or sacrificed to absorb all incoming damage for one turn.
     torch is a torch that can double as a weapon, and can be used to light things on fire (torch-fireWater combo).
     The backpack holds items that are not currently equipped. */
  vector < string > backpack {
    "gateScroll",
    "antidote",
    "healthPot",
    "stamPot",
    "fireWater",
    "smokeBomb",
    "targe",
    "torch",
    "dagger"
  }; //items that the player can use.

  /*equipped explanation:
     shortSword is the starter weapon; a shortened sword for killing kobolds. */
  vector < string > equipped {
    "shortSword"
  }; //currently equipped item.
  const vector < int > defaultHealth {
    10,
    20,
    10
  }; //holds default health values for kobolds, giant kobolds, and shamans.
  const vector < int > defaultDamage {
    10,
    20,
    30
  }; //holds default damage values for kobolds, giant kobolds, and shamans.
  const vector < string > enemyType {
    "k",
    "g",
    "s"
  }; //holds list of enemy types.
  const vector < string > enemyName {
    "kobold",
    "giant kobold",
    "shaman"
  }; //list of enemy names;
  vector < string > ::const_iterator myIter; //for checking vectors.

  //environment inspection outputs.
  const string level1Desc = "A temperate forest. A totem stands guard outside the nest."; //environment inspection output for level 1.
  const string level2Desc = "A foul-smelling tunnel. Nothing but kobold droppings and a few old bones."; //environment inspection output for level 2.
  const string level3Desc = "Another dark tunnel. A totem can be seen through the faint torch light."; //environment inspection output for level 3.
  const string level4Desc = "A party of novice adventurers besieged by three kobolds. Two lie dead; one living; with evidence of another dragged away."; //environment inspection output for level 1.
  const string level5Desc = "A narrow tunnel with three kobolds and one giant kobold."; //environment inspection output for level 5.
  const string level6Desc = "You find yourself in a large chamber with a horde of kobolds. A kobold shaman and giant kobold are among their number."; //environment inspection output for level 6.
  const string level7Desc = "You enter a small chamber. Five kobold young tremble in the darkness."; //environment inspection output for level 7.

  //general variable declarations and initializations.
  int armorDurability = 100; //tracks armor durability. Armor durability is lowered on each attack taken by half of the damage value. Armor durability determines percentage of health damage taken per hit.
  int dmgBlocked; //tracks damage blocked by armor
  const int maxHealth = 100; //max health.
  int health = maxHealth; //tracks current health.
  int miraclesLeft = 0; //number of miracles the priestess member has left. Heal and Holy Light are possible casts. Holy Light blinds enemies for one turn; blocking them from attacking. Heal heals the player for 20 health.
  const int defaultMiracles = 2; //number of miracles the priestess is granted once the player clears level 3 Revealed.
  int knives = 3; //number of throwing knives left. Deals 5 damage on enemy hit and denies that enemy an attack phase.
  int level = 0; //current level(numerical).
  const int poisonChance = 0.5; //percent chance of being poisoned per enemy attack.
  const int poisonDmg = 10; //if player is poisoned, amount of poison damage applied per turn.
  const int bAmount = 999; //determines amount of low-level enemies the 'berserk' ability can kill at once.
  const int fireDamage = 5; //determines damage per-turn of fireWater. Burns all enemies in map.
  int berserkCharges = 1; //holds number of charges for the berserk ability. Kills all lower enemies on the map.
  int playerDamage; //int variable for holding the decided PRIMARY weapon damage value.

  //weapon damage initializations.
  const int torchDamage = 5; //torch damage.
  const int shortSwordDamage = 10; //starter weapon damage.
  const int javelinDamage = 999; //javelin damage; thrown single-use weapon. Kills any enemy instantly.
  const int daggerDamage = 10; //dagger damage.
  const int hatchetDamage = 10; //hatchet damage.
  const int targeDamage = 10; //targe damage.
  const int knifeDamage = 5; //knives damage.
  const int unarmedDamage = 5; //unarmed damage.

  //boolean initializations for tracking ability uses and status effects.
  bool bActive = false; //determines whether berserk is active.
  bool holyLight = false; //determines whether enemies' attack phase will be denied.
  bool gateScrollOcean = false; //determines whether the highest level enemy in the level will be killed instantly.
  bool gateScrollCity = false; //determines whether the game ends instantly.
  bool poisoned = false; //determines whether the player is poisoned. Subtracts 10 health per turn.
  bool attackTwo = false; //determines whether two enemies of the same type will be attacked this turn.
  bool onFire = false; //determines whether all enemies in the level are on fire.
  bool confused = false; //determines whether all enemies in the level are confused.
  bool targeActive = false; //determines whether the 'targe' item is activated.
  bool denyAttack = false; //determines whether a single enemies' attack will be denied.
  bool victory = true; //determines whether the player was victorious in the quest.
  bool denyEnemAttack = false; //determines whether the enemies' attack phase will be denied.
  bool denyPlaAttack = false; //determines whether the player's attack phase will be denied.
  bool repeatIdle = false; //determines whether the level's idle phase will repeat.
  bool error = true; //determines whether a loop will end.

  //one-time inspection rewards tracker.
  bool inspO = false;
  bool inspKobold = false;

  string answer; //holds the player's current command.

  //declarations/initializations
  vector<int> variableAllHealth {defaultHealth[0], defaultHealth[1], defaultHealth[2]}; //holds variable health values for ALL kobolds, giant kobolds, and shamans in the map.
  vector<int> variableSingleHealth {variableAllHealth[0], variableAllHealth[1], variableAllHealth[2]}; //holds health values for the first enemy of each type found on the map IF deviated from default health AND > 0.
  //                                *kobold health        *giant k. health      *shaman health
  int currentEnemy; //holds the numerical value of the currently targeted enemy. 0 = k, 1 = g, 2 = s.
  int incomDmgChanges = 0; //keeps record of changes to total INCOMING damage value before it is calculated.

  srand(static_cast < unsigned int > (time(0))); //seeds the random number generator.

//current levels made of vectors of vectors. Will be editing these with various functions.
vector<vector<string>>level1
{{" ","^","^","^"," ","-","^"," ","^","^"},
{"^"," ","^","^","-","-","^",".","^","^"},
{" "," ","^","^","-","-",".","^","^","^"},
{"^"," ","^"," ","-"," ","^","^","^","^"},
{"^","^"," ","^","-","-","o","^","^","^"},
{" ","^","^","^","-","-"," "," ","^"," "},
{"^","^"," ","-","-","*"," "," ","^","^"},
{"^","^"," ","-","-"," ","t"," "," "," "},
{"_","_","_","-","-","_","_","_","_","/"},
{" "," "," ","[","]"," "," "," "," "," "},};
vector<vector<string>>level2
{{" ","["," ","]","^"},
{"."," "," "," "," "},
{" "," ","*"," ","."},
{"^"," "," "," "," "},
{"."," "," "," "," "},
{" "," "," ","'"," "},
{" ","^"," "," ","-"},
{" "," "," ","."," "},
{" ","."," "," ","^"},
{"^","["," ","]"," "}};
vector<vector<string>>level3
{{" ","["," ","]","^"},
{"^","^"," "," ","."},
{" ",".","*"," ","^"},
{"^"," "," "," ","^"},
{"^"," "," "," "," "},
{" ","."," "," ","^"},
{"^"," "," "," "," "},
{" "," "," ","t"," "},
{"^","^"," "," ","^"},
{"^","["," ","]","^"}};
vector<vector<string>>level3Reveal
{{" ","["," ","]","^"},
{"^","^"," "," ","."},
{" ",".","*"," ","^"},
{"^"," "," "," ","^"},
{"^"," "," "," "," "},
{" ",".","k"," ","^"},
{"^"," "," ","k"," "},
{" ","k"," ","t"," "},
{"^","^"," "," ","^"},
{"^","["," ","]","^"}};
vector<vector<string>>level4
{{"^","^","["," ","]"," ","^"},
{" ","^"," "," "," ","^","^"},
{" "," "," ","*"," "," ","^"},
{"^"," ","x"," "," "," ","."},
{"^"," "," "," ","x"," ","^"},
{"^"," ","k"," "," "," ","^"},
{"^","k","p"," "," "," ","^"},
{" "," "," "," ","k","^","^"},
{"^","."," "," "," ","^","^"},
{"^","^","["," ","]","^","^"}};
vector<vector<string>>level5
{{"^","["," ","]","^"},
{"^","p"," "," "," "},
{" "," ","*"," ","^"},
{"^"," "," "," ","^"},
{"^"," "," "," ","^"},
{"^",".","g"," ","^"},
{" "," "," "," ","^"},
{"^"," ","k","k"," "},
{"^","k"," "," ","^"},
{"^","["," ","]","^"}};
vector<vector<string>>level6
{{"^","^","^","^"," "," ","["," ","]"," ","^","^","^","^","^"},
{"^","^"," ","^"," "," "," "," "," "," ","^"," "," "," ","^"},
{"^","^"," "," "," "," "," "," "," "," "," "," "," "," "," "},
{"^"," "," "," "," "," ","p"," "," "," "," "," "," "," ","^"},
{"^"," ","^"," ","^"," "," "," "," "," ","^"," "," ","^","^"},
{"^","^"," "," "," "," "," ","*"," "," "," "," "," ","^","^"},
{"^","^","^"," ","^"," "," "," "," "," "," "," "," "," ","^"},
{"^"," "," "," "," "," "," "," "," ","^"," "," "," "," ","^"},
{"^"," "," "," ","k"," "," "," "," ","^"," ","k"," "," ","^"},
{"^"," "," "," ","k","k"," "," "," ","k"," "," "," "," ","^"},
{"^","^"," "," "," "," "," "," "," "," "," ","k"," ","^","^"},
{"^","^"," ","^","g"," "," "," "," "," "," "," "," "," ","^"},
{"^","^"," "," "," "," "," "," ","k"," "," "," "," ","^","^"},
{"^","^","^"," "," "," "," ","s"," "," "," "," ","^","^","^"},
{"^","^","^","^"," ","^","["," ","]","^","^","^","^","^","^"}};
vector<vector<string>>level7
{{"_","["," ","]","_"},
{" "," ","*"," "," "},
{" "," "," "," "," "},
{" ","k","k"," "," "},
{" ","k","k","k"," "}};

  //introduction to the game.
  introAndInstruct(poisonChance, equipped, fireDamage, knifeDamage); //prints introduction and instructions.

  for (level = 0; level != 7 && (health > 0); level++) //level-based game loop. Using a for-loop and a counter to increment level by level.
  {

    vector<vector<string>>& currentLevel = getLevel(level, level1, level2, level3, level3Reveal, level4, level5, level6, level7); //gets the current level based on the counter variable 'level.'
    inspKobold = false; //resets the inspect kobold bool. Keeps track of whether a kobold has been searched for weapons on this level.
    
    do { //Repeat the level while health is greater than zero AND (enemies on level OR player wants to continue the idle phase)
      repeatIdle = false; //Determines whether the game loop will be repeated. This resets the variable to false initially.
      if (level == 4) //if the level = level3Reveal, the priestess is gained, so set the starter amount of miracles equal to the default amount.
      {
        miraclesLeft = defaultMiracles; //set the default number of miracles for the accompanying priestess.
      }
    
    //idle phase.
    idlePhase(maxHealth, level, level1Desc, level2Desc, level3Desc, level4Desc, level5Desc, level6Desc, level7Desc, backpack, inspO, inspKobold, denyAttack, denyEnemAttack, answer, health, holyLight, miraclesLeft, knives, knifeDamage, berserkCharges, bActive, gateScrollCity, gateScrollOcean, poisoned, attackTwo, onFire, confused, targeActive, currentLevel, enemyType, armorDurability, defaultDamage, equipped); //idle phase main function
      
//MAKE COMBAT PHASE A FUNCTION
    
      //enter combat phase.
    
//MAKE ABILITY CALCULATION A FUNCTION
      
      //calculate incoming and outgoing ABILITY and buff/debuff damage. Removes enemies from the map if killed(health value at 0 or less).
      if (bActive == true) //kills all low-level enemies in the level.
      {
        repeatIdle = true;
        killEnem(currentLevel, enemyType[0], bAmount, enemyName);
        cout << "The words of your master ring in your ears." << endl <<
          "'You think you can slay kobolds?'" << endl <<
          "'You sat and watched quietly.'" << endl <<
          "'You were powerless. You didn't do a thing.'" << endl <<
          "Enraged, and with impossible speed, you dispatch every lower kobold in the room." <<
          endl;
        bActive = false; //deactivates berserk ability.
      }
      if (holyLight == true) {
        denyEnemAttack = true; //denies the enemy attack phase.
        cout << "The priestess raises her staff." << endl <<
          "Holy light blinds all enemies in the room." << endl;
        holyLight = false; //deactivates Holy Light ability
      }
      if (gateScrollOcean == true) {
        cout <<
          "You have chosen to unleash the power of the unbridled ocean." <<
          endl;
        error = true;
        while (error = true) {
          cout << "Type '_' to kill one enemy of that type instantly." <<
            endl;
          getAnswer(answer);
          myIter =
            find(enemyType.begin(), enemyType.end(), answer);
          if (myIter != enemyType.end()) {
            killEnem(currentLevel, answer, 1, enemyName); //Instantly kills the targeted enemy, regardless of health.
            cout <<
              "Sliced to pieces by salt water, a kobold dies." <<
              endl;
            cout << "The scroll burns up in your hand." << endl;
            gateScrollOcean = false; //deactivates gateScrollOcean.
            printLevel(currentLevel); //prints current level.
            error = false;
            repeatIdle = true;
          } else {
            error = true;
            cout << "No enemy of this type found." << endl;
          }
        }
      }
      if (gateScrollCity == true) {
        victory == false;
        return 0; //exits the function immediately, as game is lost.
        //gateScrollCity and victory variables are used to determine the defeat text.
      }
      if (poisoned == true) {
        health = health - poisonDmg; //applies poison damage to player if poisoned.
        if (health < 0) {
          cout << "The kobolds' cruel poison takes effect." << endl <<
            "You slip into unconsciousness." << endl;
          return 0;
        }
      }
      if (attackTwo == true) {
        cout << "You will take 50 percent less damage and hit for double damage in the next phase." << endl;
      }
      if (onFire == true) //lowers the health of ALL enemies by fireDamage, and lowers the health of already-damaged enemies by fireDamage. Deactivates when a new level is entered.
      {
        variableSingleHealth[0] = variableSingleHealth[0] - fireDamage;
        variableSingleHealth[1] = variableSingleHealth[1] - fireDamage;
        variableSingleHealth[2] = variableSingleHealth[2] - fireDamage;
        variableAllHealth[0] = variableAllHealth[0] - fireDamage;
        variableAllHealth[1] = variableAllHealth[1] - fireDamage;
        variableAllHealth[2] = variableAllHealth[2] - fireDamage;
        for (int i = 0; i < 3; i++) //checks all directly-attacked enemies for death.
        {
          if (variableSingleHealth[i] <= 0) //checks if any of the health values in variableSingleHealth are equal to or less than zero.
          {
            variableSingleHealth[i] = variableAllHealth[i]; //resets health to the default health of that enemy type in the level.
            killEnem(currentLevel, enemyType[i], 1, enemyName); //removes one enemy of that type from the level.
            cout << "A " << enemyName[i] << " burned to death." <<
              endl;
            printLevel(currentLevel); //prints current level.
            repeatIdle = true;
          }
        }
        for (int i = 0; i < 3; i++) //checks for death of a whole enemy type on the level.
        {
          if (variableAllHealth[i] <= 0) //checks if any of the health values in variableAllHealth are equal to or less than zero.
          {
            //health of each value in variableAllHealth is not reset to default after death, as there are no other enemies of that type left alive in the level.
            killEnem(currentLevel, enemyType[i], 999, enemyName); //removes ALL enemies of that type from the level.
            cout << "All " << enemyName[i] <<
              "s have burned to death." << endl;
            printLevel(currentLevel); //prints current level.
            repeatIdle = true;
          }
        }
      }
      if (confused == true) //denies enemies' attack phase if effected by the smokeBomb item.
      {
        denyEnemAttack = true; //denies the enemy attack phase.
        cout << "All enemies in the room become confused!" << endl;
        confused = false; //deactivates debuff from smokeBomb item.
      }
      if (targeActive == true) //denies enemy attack phase, but destroys your shield.
      {
        denyEnemAttack = true;
        cout << "Your shield shatters under the kobolds' attacks." <<
          endl;
        targeActive = false; //deactivates targeActive ability.
      }
      if (denyAttack == true) //denies the attack phase of ONE enemy and applies knife damage.
      {

        cout <<
          "You scan the room for an appropriate target for your knife..." <<
          endl;
        for (error = true; error == true;) {
          error = false;
          cout << "Type '_' to throw your knife at that enemy." <<
            endl;
          getAnswer(answer); //getting an answer.
          int selectedEnem; //holds the numerical value of the selected enemy.
          myIter =
            find(enemyType.begin(), enemyType.end(), answer);
          if (myIter != enemyType.end()) {
            if (answer == enemyType[0]) {
              selectedEnem = 0; //0 for kobold.
            } else if (answer == enemyType[1]) {
              selectedEnem = 1; //1 for giant kobold.
            } else if (answer == enemyType[2]) {
              selectedEnem = 2; //2 for shaman.
            }
            variableSingleHealth[selectedEnem] = variableSingleHealth[selectedEnem] - knifeDamage; //applies knife damage to that enemy.
            if (variableSingleHealth[selectedEnem] <= 0) {
              cout << "Your knife strikes the weakened " <<
                enemyName[selectedEnem] <<
                " with incredible speed." << endl <<
                "It dies instantly. Blood pools on the floor." <<
                endl;
              variableSingleHealth[selectedEnem] = variableAllHealth[selectedEnem]; //resets health to the default health of that enemy type in the level.
              killEnem(currentLevel, enemyType[selectedEnem], 1, enemyName); //removes one enemy of that type from the level.
              printLevel(currentLevel); //prints current level.
              denyAttack = false;
              repeatIdle = true;
            } else {
              cout << "You throw your knife; it hits the enemy." <<
                endl;
              cout << "That enemy cannot attack this turn." <<
                endl;
              incomDmgChanges = incomDmgChanges - defaultDamage[selectedEnem]; //reduced incoming damage by one enemy's worth.
              denyAttack = false;
              repeatIdle = true;
              printLevel(currentLevel); //prints current level.
            }
          } else {
            error = true;
            cout << "No enemy of this type found." << endl;
          }
        }
        cout <<
          "_ _ _____________________________________________________________ _ _" <<
          endl;
      }
//MAKE ATTACK SUB-PHASE A FUNCTION
      if (dmgCalc(currentLevel, defaultDamage, enemyType) > 0 && (health > 0)) { //if enemies exist, enter combat phase.
//MAKE SELECT CORRECT ENEMY A FUNCTION
        //select correct enemy loop for PRIMARY weapon damage.
        for (error = true; error == true;) //set error = true and set the loop to repeat if an error is encountered.
        {
          error = false; //reset error variable.
          cout << "Type 'attack _' to attack that enemy." << endl;
          getAnswer(answer); //gets answer for enemy to attack.
          answer.erase(0, 7); //formatting answer.
          vector < int > vec = findObjPos(currentLevel, answer); //returns enemy positions to vector; if any.
          myIter = find(enemyType.begin(), enemyType.end(), answer); //checks if the char entered matches an appropriate enemy type.
          if (vec.empty()) //checks if vec is empty.
          {
            cout << "There is no enemy of this type." << endl;
            error = true;
          } else if (myIter != enemyType.end()) {
            if (answer == enemyType[0]) {
              currentEnemy = 0; //0 for kobold.
            } else if (answer == enemyType[1]) {
              currentEnemy = 1; //1 for giant kobold.
            } else if (answer == enemyType[2]) {
              currentEnemy = 2; //2 for shaman.
            }
          } else {
            cout << "You cannot attack that!" << endl;
            error = true;
          }
        }
//MAKE EQUIPPED DAMAGE CALCULATION A FUNCTION
        if (equipped.empty() == false) {
          //assigning correct damage value for primary weapon equipped.
          if ((health > 0) && (equipped[0].find("javelin") != string::npos)) {
            playerDamage = javelinDamage;
            equipped.pop_back();
            cout <<
              "You throw the javelin, instantly killing the enemy." <<
              endl;
          } else if ((health > 0) && (equipped[0].find("shortSword") != string::npos)) {
            playerDamage = shortSwordDamage;
          } else if ((health > 0) && (equipped[0].find("dagger") != string::npos)) {
            playerDamage = daggerDamage;
          } else if ((health > 0) && (equipped[0].find("hatchet") != string::npos)) {
            playerDamage = hatchetDamage;
          } else if ((health > 0) && (equipped[0].find("torch") != string::npos)) {
            playerDamage = torchDamage;
          } else if ((health > 0) && (equipped[0].find("targe") != string::npos)) {
            playerDamage = targeDamage;
          }
        } else if (equipped.empty() == true) {
          cout << "You need to find a new weapon soon. You are unarmed." << endl;
          playerDamage = unarmedDamage;
        }
//MAKE STAMPOT BUFF A FUNCTION
        if (attackTwo == true) //stamPot buff calculation
        {
          playerDamage = playerDamage * 2;
        }

        cout <<
          "_ _ _____________________________________________________________ _ _" <<
          endl;
        cout << endl;
        
//MAKE BLOODSOAKED DEBUFF CALCULATION A FUNCTION
        //keeps track of the bloodSoaked debuff progression and calculates PRIMARY damage value(s)
        if (equipped.empty() == false) {
          if ((equipped[0].find("bloodSoaked ") != string::npos) && (health > 0)) {
            int slipChance = ((rand() % 10) + 1); //get a number between 1 and 10
            if (slipChance > 1) //90% chance of slippage resulting in zero damage.
            {
              cout << "The " << equipped[0] <<
                " slips from your hand and tumbles into the darkness." <<
                endl;
              equipped.erase(equipped.begin()); //removes the weapon from equipped.
              cout << "You strike the " << enemyName[currentEnemy] <<
                " for " << unarmedDamage << " unarmed damage." <<
                endl;
              variableSingleHealth[currentEnemy] =
                variableSingleHealth[currentEnemy] - unarmedDamage;
              repeatIdle = true;
            } else {
              cout <<
                "The Slayer won't let anyone roll the dice." << endl;
              cout << "You strike the " << enemyName[currentEnemy] <<
                " for " << playerDamage << " damage." << endl;
              variableSingleHealth[currentEnemy] =
                variableSingleHealth[currentEnemy] - playerDamage;
              cout << "Blood spills from your " << equipped[0] << "." <<
                endl;
              berserkCharges = berserkCharges + 1; //Secretly adds a charge to the berserk ability.
            }
          } else if ((equipped[0].find("bloodStained ") != string::npos) && (health > 0)) {

            variableSingleHealth[currentEnemy] =
              variableSingleHealth[currentEnemy] - playerDamage;
            cout << "You strike the " << enemyName[currentEnemy] <<
              " for " << playerDamage << " damage." << endl;
            cout << endl << "Your weapon is slick with blood." << endl <<
              "Slippage during battle is highly likely." << endl;
            equipped[0] = "bloodSoaked " + (equipped[0].erase(0, 13)); //changes name of equipped weapon to reflect debuff progression.
          } else if (health > 0) {
            cout << "You strike the " << enemyName[currentEnemy] <<
              " for " << playerDamage << " damage." << endl;
            variableSingleHealth[currentEnemy] =
              variableSingleHealth[currentEnemy] - playerDamage;
            equipped[0] = "bloodStained " + equipped[0]; //changes name of equipped weapon to reflect debuff progression.
            cout << endl << "Your weapon is smeared with blood." << endl <<
              "You can probably kill one more enemy with this." <<
              endl;
          }
        } else if (health > 0) {
          cout << "You strike the " << enemyName[currentEnemy] <<
            " for " << unarmedDamage << " unarmed damage." <<
            endl;
          variableSingleHealth[currentEnemy] =
            variableSingleHealth[currentEnemy] - unarmedDamage;
          repeatIdle = true;
        }
        cout << "_ _ _____________________________________________________________ _ _" << endl;
        
        repeatIdle = removeDead(currentEnemy, variableSingleHealth, variableAllHealth, currentLevel, enemyType, enemyName); //removes dead enemies from the map and re-assigns variableSingleHealth value; also sets the repeatIdle variable to true if an enemy was removed. Repeats the phase if an enemy was removed by the ability sub-phase.
        
//MAKE INCOMING DAMAGE SUB-PHASE A FUNCTION
        
        //applies incoming damage, if any.
        if ((denyEnemAttack != true) && (health > 0)) {
          int incomingDamage =
            dmgCalc(currentLevel, defaultDamage, enemyType);
          if (attackTwo == true) //stamPot buff
          {
            incomingDamage = incomingDamage / 2;
          }
          dmgBlocked = ((armorDurability / 100) * incomingDamage); //determines amount of damage blocked.
          armorDurability = (armorDurability - (incomingDamage / 2)); //degrades armour by 50% of damage taken.
          health = health - (incomingDamage - dmgBlocked); //applies damage to the player's health.
          //applies poisoned effect
          if ((poisoned == false)) {
            int poisonDie = ((rand() % 10) + 1); //generates a random number from 1 - 10;
            if (poisonDie < poisonChance) {
              poisoned = true;
              cout <<
                "You've been poisoned by the kobolds' attack." <<
                endl << "Apply an antidote quickly!" << endl;
            }
          }
          cout << "The kobolds hit you for " << (incomingDamage -
              dmgBlocked) <<
            " damage." << endl << "Your armour loses " <<
            (incomingDamage /
              2) << " percent durability," << endl <<
            "and currently has a durability percentage of " <<
            armorDurability << "." << endl;
          cout <<
            "_ _ _____________________________________________________________ _ _" <<
            endl;
        } else if (health > 0) {
          cout << "The kobolds' attack phase was denied." << endl;
          cout <<
            "_ _ _____________________________________________________________ _ _" <<
            endl;
          denyEnemAttack = false;
        }
      }
    }
    while ((health > 0) && ((dmgCalc(currentLevel, defaultDamage, enemyType) > 0) ||
        (repeatIdle == true)));
  }

//MAKE ENDGAME A FUNCTION

  //endgame; either death or reach level 7.
  if (health <= 0) {
    cout << endl << "You died from lack of health!" << endl;
  } else {
    printLevel(level7);

    cout << "You enter the back room." << endl;
    cout << "It's a tiny space." << endl;
    cout << "a few kobold young huddle in the middle of the room." << endl << endl;
    cout << "You've scoured the land for kobolds, because they destroyed your home;" << endl;
    cout << "Your village; Your friends; Your family." << endl;
    cout << "Kobolds hold onto grudges for life, and they learn from their mistakes." << endl;
    cout << "If you leave these alive, they will destroy more villages; and take more lives." << endl << endl;
    cout << "Do you kill them?" << endl << "Enter 'yes' or 'no'" << endl;
    getAnswer(answer);

    for (int error = true; error == true;) {

      if (answer == "yes") {
        error = false;
        victory = true;
      } else if (answer == "no") {
        error = false;
        victory = false;
      } else {
        cout << "Enter 'yes' or 'no'" << endl;
        error = true;
      }
    }

    if (victory == true) {
      cout << "You emerge from the hole." << endl << "covered in blood, you make your way back to the guild hall to collect your reward." << endl;
    } else {
      cout << "As you walk away, one of the juvenile kobolds attacks you with a club" << endl;
      cout << "You can hear their maniacal cackling as your senses fade..." << endl;
    }
  }
}

//function definitions.

//prints current level. (with changed based on gameplay progression)
void
printLevel(vector < vector < string >> grid) {
  cout << endl;
  for (unsigned int i = 0; i != grid.size(); i++) {
    cout << "     ";
    for (unsigned int j = 0; j != grid[0].size(); j++) {
      cout << grid[i][j];
    }
    cout << endl;
  }
}

//finds the position of a certain object in a certain level.
vector < int >
  findObjPos(vector < vector < string >> & grid, string entered) {
    vector < int > coordinates;
    for (unsigned int i = 0; i != grid.size(); i++) {
      for (unsigned int j = 0; j != grid[0].size(); j++) {
        if (entered == grid[i][j]) {
          coordinates.push_back(i);
          coordinates.push_back(j);
          return coordinates;
        }
      }
    }
  }

//kills a certain number of enemies of a type in the current level.
void
killEnem(vector < vector < string >> & grid, string enemyType, int numEnemies,
  const vector < string > enemyName) {
  cout << "_ ______________________ _" << endl << endl;
  vector < int > coordinates; //holds coordinates of one enemy.
  bool end = false; //determines whether the loop will end (out of enemies).
  for (unsigned int i = 0;
    (i < numEnemies) || (end = false); i++) {
    coordinates = findObjPos(grid, enemyType); //get coordinates for one enemy.
    if (coordinates.empty()) {
      end = true;
    } else {
      cout << "Killed " << grid[coordinates[0]][coordinates[1]] << "!" <<
        endl;
      grid[coordinates[0]][coordinates[1]] = "#";
    }
  }
  cout << "_ _ __________________ _ _" << endl;
}

//takes the number of enemies of each type in the level and calculates total damage figure.
int
dmgCalc(vector < vector < string >> grid,
  const vector < int > defaultDamage,
    const vector < string > enemyType) {
  int totalDamage = 0; //holds total damage value. (initialization)

  for (unsigned int i = 0; i < 3; i++) //increments enemy type and damage.
    for (unsigned int j = 0; j != grid.size(); j++) {
      for (unsigned int k = 0; k != grid[0].size(); k++) {
        if (enemyType[i] == grid[j][k]) {
          totalDamage = totalDamage + defaultDamage[i];
        }
      }
    }
  return totalDamage; //returns the total damage value.
}

//calculates the number of enemies in a level.
vector < int >
  enemCalc(vector < vector < string >> grid,
    const vector < string > enemyType) {
    vector < int > totalEnemies {
      0,
      0,
      0
    }; //total amount of each enemy in the order of k, g, s.
    for (unsigned int i = 0; i < 3; i++) //increments enemy type
      for (unsigned int j = 0; j != grid.size(); j++) {
        for (unsigned int k = 0; k != grid[0].size(); k++) {
          if (enemyType[i] == grid[j][k]) {
            totalEnemies[i]++;
          }
        }
      }
    return totalEnemies; //returns the total damage value.
  }

//gets the user to enter a command.
void
getAnswer(string & myAnswer) {
  cout << "_ _ ______________ _ _" << endl;
  cout << endl;
  cout << "Please enter a command: ";
  getline(cin, myAnswer);
}

//gets the current level based on counter variable 'level' and prints a short introduction.
vector < vector < string >> & getLevel(int counter,
  vector < vector < string >> & level1,
  vector < vector < string >> & level2,
  vector < vector < string >> & level3,
  vector < vector <
  string >> & level3Reveal,
  vector < vector < string >> & level4,
  vector < vector < string >> & level5,
  vector < vector < string >> & level6,
  vector < vector < string >> & level7) {
  switch (counter) {
  case 0:
    //cout << "Entering level 1." << endl;
    return level1;
    break;
  case 1:
    //cout << "Entering level 2." << endl;
    return level2;
    break;
  case 2:
    //cout << "Entering level 3." << endl;
    return level3;
    break;
  case 3:
    //cout << "Entering Level 3 Revealed." << endl;
    return level3Reveal;
    break;
  case 4:
    //cout << "Entering Level 4." << endl;
    return level4;
    break;
  case 5:
    //cout << "Entering Level 5." << endl;
    return level5;
    break;
  case 6:
    //cout << "Entering Level 6." << endl;
    return level6;
    break;
  case 7:
    //cout << "Entering Level 7." << endl;
    return level7;
    break;
  }
}

//command for equipping weapons from backpack and unequipping weapons from equipped.
void
equip(string answer, vector < string > & backpack,
  vector < string > & equipped) {
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
  cout << endl;
  vector < string > ::iterator myIter; //for swapping weapons.
  answer.erase(0, 6); //formatting answer.
  string temp; //for swapping weapon.
  if (find(backpack.begin(), backpack.end(), answer) != backpack.end()) //finding answer in backpack.
  {
    if (answer.find("shortSword") != string::npos || answer.find("javelin") != string::npos || answer.find("dagger") != string::npos || answer.find("hatchet") != string::npos || answer.find("targe") != string::npos || answer.find("torch") != string::npos) //finding answer that corresponds with a weapon type.
    {
      if (answer.find("Soak") != string::npos) //tests if weapon is bloodSoaked.
      {
        cout << "This weapon is slick with blood." << endl <<
          "Slippage during battle is highly likely!" << endl << endl;
      } else if (answer.find("Stain") != string::npos) //tests if weapon is bloodStained.
      {
        cout << "This weapon is smeared with blood." << endl <<
          "You can probably kill one more enemy with this." << endl;
      }
      myIter = find(backpack.begin(), backpack.end(), answer);
      if (equipped.empty()) //if equipped is empty.
      {
        equipped.push_back( * myIter);
        backpack.erase(myIter);
      } else //if equipped holds a weapon.
      {
        temp = equipped[0];
        equipped[0] = * myIter;
        backpack.erase(myIter);
        backpack.push_back(temp);
      }
      cout << equipped[0] << " has been equipped." << endl;
      cout << temp << " has been transferred to your backpack." << endl;
    } else {
      cout << "You cannot equip this item." << endl;
    }
  } else {
    cout << "Item not found." << endl;
  }
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
}

//command for inspecting objects and levels.
void
inspect(string answer, vector < vector < string >> currentLevel, int level,
  const string level1Desc,
    const string level2Desc,
      const string level3Desc,
        const string level4Desc,
          const string level5Desc,
            const string level6Desc,
              const string level7Desc, vector < string > & backpack, bool & inspO,
                bool & inspKobold, bool & denyAttack, bool & denyEnemAttack) {
  answer.erase(0, 8); //formatting answer.
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
  cout << endl;
  if (answer == "environment") //for inspecting levels.
  {
    switch (level) {
    case 0:
      cout << level1Desc << endl;
      break;
    case 1:
      cout << level2Desc << endl;
      break;
    case 2:
      cout << level3Desc << endl;
      break;
    case 3:
      cout << level3Desc << endl;
      break;
    case 4:
      cout << level4Desc << endl;
      break;
    case 5:
      cout << level5Desc << endl;
      break;
    case 6:
      cout << level6Desc << endl;
      break;
    case 7:
      cout << level7Desc << endl;
      break;
    }
  } else //for inspecting objects.
  {
    //prep for testing whether the current level is empty of the entered object.
    vector < int > vec;
    vec = findObjPos(currentLevel, answer);

    if (vec.empty()) {
      cout << "This object does not exist." << endl;
    } else {
      if (answer == "t") {
        cout << "A totem." << endl;
        if (level == 2) {
          denyEnemAttack = true;
          cout << endl <<
            "Kobolds use these totems to distract adventurers." <<
            endl << "This is an ambush." << "You ready your weapon." <<
            endl << endl <<
            "(The kobolds' next attack phase will be blocked.)" <<
            endl;
        }
      } else if (answer == "o") {
        cout << "A tree stump." << endl;
        if (inspO == false) {
          cout << "You find a hatchet." << endl;
          backpack.push_back("hatchet");
          inspO = true;
        }
      } else if (answer == ".") {
        cout << "Kobold droppings." << endl;
      } else if (answer == "-") {
        cout << "A cracked bone." << endl;
      } else if (answer == "x") {
        cout << "A dead adventurer." << endl;
      } else if (answer == "k") {
        cout << "A foul-smelling kobold." << endl;
      } else if (answer == "c") {
        cout << "Kobold young." << endl;
      } else if (answer == "g") {
        cout << "A giant kobold." << endl;
      } else if (answer == "s") {
        cout << "A kobold shaman." << endl;
      } else if (answer == "p") {
        cout << "A holy priestess." << endl;
        cout << "She will aid you on this quest." << endl;
      } else if (answer == "#") {
        cout << "A dead kobold." << endl;
        if (inspKobold == false) {
          int weaponChance = ((rand() % 4) + 1); //setting up weaponChance to contain a random number from 1 - 4.
          switch (weaponChance) //gives a random weapon based on weaponChance.
          {
          case 1:
            backpack.push_back("shortSword");
            cout << "You found a shortSword on its' corpse!" <<
              endl;
            break;
          case 2:
            backpack.push_back("javelin");
            cout << "You found a javelin on its' corpse!" <<
              endl;
            break;
          case 3:
            backpack.push_back("dagger");
            cout << "You found a dagger on its' corpse!" <<
              endl;
            break;
          case 4:
            backpack.push_back("hatchet");
            cout << "You found a hatchet on its' corpse!" <<
              endl;
            break;
          }
          inspKobold = true; //sets the bool == true so no more weapons can be obtained from one level.
        } else {
          cout << "You find nothing on its' body." << endl;
        }
      } else if (answer == "*") {
        cout << "Tempered by the fires of his youth," << endl <<
          "his iron will remains steadfast through the passage that preys upon the weak." <<
          endl <<
          "He had set forth without pity, and hunts the kobolds with barbarous cruelty." <<
          endl << "Unbreakable, incorruptible, unyielding." << endl <<
          "Burned by the embers of his rage," << endl <<
          "his soul tainted by the scars of countless battles," << endl <<
          "he chose the path of perpetual slaughter." << endl <<
          "In his ravenous hatred he found no peace;" << endl <<
          "and with boiling blood he scoured the land;" << endl <<
          "seeking vengeance against the kobolds who had wronged him." <<
          endl << "He wore the pallor of an age lived in darkness," <<
          endl <<
          "and those who witnessed the depth of his fixation named him the Kobold Slayer." <<
          endl;
      } else {
        cout << "You cannot inspect that object." << endl;
      }
    }
  }
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
}

//command for using items or abilities.
void
use(string & answer, int & health, int maxHealth, bool & holyLight,
  int & miraclesLeft, int & knives, int knifeDamage, bool & denyAttack,
  int & berserkCharges, bool & bActive, vector < string > & backpack,
  vector < string > equipped, bool & gateScrollCity,
  bool & gateScrollOcean, bool & poisoned, bool & attackTwo, bool & onFire,
  bool & confused, bool & targeActive,
  vector < vector < string >> currentLevel, vector < string > enemyType,
  int armorDurability, vector < int > defaultDamage) {
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
  cout << endl;
  bool error = true; //Ends the loop once an appropriate action is taken.
  /*if (dmgCalc (currentLevel, defaultDamage, enemyType) <= 0)	//checks if vec is empty of enemies.
    {
      cout <<
	"There are no enemies in this level. Ability and item usage is blocked."
	<< endl;
      answer = "quit";
    }*/
  while (error == true && answer != "quit") {
    error = false;
    answer.erase(0, 4);
    if ((answer == "holyLight") && (miraclesLeft > 0) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      holyLight = true;
      miraclesLeft = miraclesLeft - 1;
      cout << "holyLight activated." << endl;
    } else if ((answer == "heal") && (miraclesLeft > 0)) {
      if (health <= 80) {
        health = health + 20;
        cout << "Healed for 20 health." << endl;
      } else {
        health = (health + (maxHealth - health)); //heals up to the max health.
        cout << "Healed to 100 health." << endl;
      }
      miraclesLeft = miraclesLeft - 1;
    } else if ((answer == "knives") && (knives > 0) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      denyAttack = true;
      knives = knives - 1;
      cout << "You ready your knife." << endl;
    } else if ((answer == "berserk") && (berserkCharges > 0) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      berserkCharges = berserkCharges - 1; //decrements berserk charges.
      bActive = true; //activates berserk buff for the combat phase.
      cout << "Berserk activated." << endl;
    } else if ((answer == "gateScroll") && (find(backpack.begin(), backpack.end(), "gateScroll") != backpack.end()) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {

      cout << "You may select a gate location." << endl <<
        "Your choices are:" << endl <<
        "1. 'city' (escape from the cave; game over.)" << endl <<
        "2. 'ocean' (opens a gate to the" << endl <<
        "bottom of the ocean; killing the" << endl <<
        "highest rank enemy in the level.)" << endl << endl;
      cout << "Type 'city' or 'ocean'." << endl;

      error = true; //resetting error.
      while (error == true && answer != "quit") {
        getAnswer(answer);

        if (answer == "city") {
          gateScrollCity = true;
          cout << "You write 'city' on the gate scroll." << endl;
          backpack.erase(find(backpack.begin(), backpack.end(),
            "gateScroll"));
          error = false;
        } else if (answer == "ocean") {
          gateScrollOcean = true;
          cout << "You write 'ocean' on the gate scroll." << endl;
          backpack.erase(find(backpack.begin(), backpack.end(),
            "gateScroll"));
          error = false;
        } else {
          cout << "Command canceled." << endl <<
            "Type 'city' or 'ocean' or type 'quit'" << endl <<
            "to return to the idle phase options." << endl;
          error = true;
        }
      }
      error == false;
    } else if ((answer == "antidote") && (find(backpack.begin(), backpack.end(), "antidote") != backpack.end())) {
      if (poisoned == false) {
        cout << "You are not poisoned." << endl;
        error = true;
        getAnswer(answer);
      } else {
        poisoned = false;
        cout << "You drink the antidote." << endl;
        backpack.erase(find(backpack.begin(), backpack.end(),
          "antidote"));
      }
    } else if (answer == "healthPot" &&
      find(backpack.begin(), backpack.end(),
        "healthPot") != backpack.end()) {
      health = (health + (maxHealth - health)); //heals up to the max health.
      cout << "You drink the health potion." << endl;
      backpack.erase(find(backpack.begin(), backpack.end(), "healthPot"));
    } else if (answer == "stamPot" &&
      find(backpack.begin(), backpack.end(),
        "stamPot") != backpack.end() && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      cout << "You drink the stamina potion." << endl;
      attackTwo = true;
      backpack.erase(find(backpack.begin(), backpack.end(), "stamPot"));
    } else if (answer == "fireWater" &&
      find(backpack.begin(), backpack.end(),
        "fireWater") != backpack.end() &&
      ((find(backpack.begin(), backpack.end(), "torch") !=
          backpack.end()) ||
        (find(equipped.begin(), equipped.end(), "torch") !=
          equipped.end())) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      cout << "You toss the open bag of fire water at the ceiling." <<
        endl;
      cout << "It bursts, spraying the kobolds with liquid fire." << endl;
      onFire = true;
      backpack.erase(find(backpack.begin(), backpack.end(), "fireWater"));
    } else if (answer == "smokeBomb" &&
      find(backpack.begin(), backpack.end(),
        "smokeBomb") != backpack.end() && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      cout << "You dash the smokebomb against the ground." << endl;
      confused = true;
      backpack.erase(find(backpack.begin(), backpack.end(), "smokeBomb"));
    } else if (answer == "targe" &&
      ((find(backpack.begin(), backpack.end(), "targe") !=
          backpack.end()) ||
        (find(equipped.begin(), equipped.end(), "targe") !=
          equipped.end())) && (dmgCalc(currentLevel, defaultDamage, enemyType) > 0)) {
      cout << "You ready your shield against the kobolds' fury." << endl;
      targeActive = true;
      backpack.erase(find(backpack.begin(), backpack.end(), "targe"));
      backpack.erase(find(equipped.begin(), equipped.end(), "targe"));
    } else {
      cout << "Command canceled." << endl <<
        "Re-type your command or type 'quit'" << endl <<
        "to advance to the next phase." << endl;
      getAnswer(answer);
      error = true;
    }
  }
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
}

//prints the contents of backpack, prints ability and item status.
void
inventory(vector < string > backpack, vector < string > equipped,
  int miraclesLeft, int knives, int berserkCharges, int level) {
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
  cout << endl;
  cout << "The contents of your backpack are: " << endl;
  for (unsigned int i = 0; i != backpack.size(); i++) {
    cout << i + 1 << ". " << backpack[i] << endl;
  }

  if (equipped.empty() == false) {
    cout << endl << "Currently equipped Weapon: " << equipped[0] << endl <<
      endl;
  }

  if ((miraclesLeft > 0) && (level > 3)) {
    cout << "Your priestess has " << miraclesLeft << " miracles left." <<
      endl << "She can use 'heal' or 'holyLight'" << endl <<
      "'heal' heals the Slayer for 20 health." << endl <<
      "'holyLight' denies all enemies in the level an attack phase." << endl <<
      endl;
  } else if ((miraclesLeft == 0) && (level > 3)) {
    cout << "Your priestess is out of spells." << endl << endl;
  }

  if (knives > 0) {
    cout << "You have " << knives << " knives left." << endl <<
      "Type 'use knives' to throw a knife at the targeted enemy in the idle phase." <<
      endl << endl;
  } else {
    cout << "You are out of knives." << endl << endl;
  }

  if (berserkCharges > 0) {
    cout << "The Slayer has an ability, 'berserk' left." << endl <<
      "Use this ability in the idle phase to kill all" << endl <<
      "low level-enemies in the current room. Use wisely." << endl;
    if (berserkCharges > 1) {
      cout << "You have " << berserkCharges << " charges left." << endl;
    }
    cout <<
      "_ _ _____________________________________________________________ _ _" <<
      endl;
  }
}

//checks player health.
void
checkHealth(int health,
  const int & maxHealth, int armorDurability) {
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
  cout << endl;
  cout << "Your current health is " << health << "/" << maxHealth << "." <<
    endl;
  cout << "Your armor currently has a durability percentage of " <<
    (armorDurability) << "." << endl;
  cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
}

bool removeDead(int currentEnemy, vector<int>& variableSingleHealth, vector<int> variableAllHealth, vector<vector<string>>& currentLevel, const vector<string> enemyType, const vector<string> enemyName) //removes dead enemies from the map and re-assigns variableSingleHealth value.
{
    if (variableSingleHealth[currentEnemy] <= 0) //if any enemies have 'dead' health
    {
        variableSingleHealth[currentEnemy] = variableAllHealth[currentEnemy]; //resets health to the default health of that enemy type in the level.
        killEnem(currentLevel, enemyType[currentEnemy], 1, enemyName); //removes one enemy of that type from the level.
        return true; //if an enemy was found
    }
    return false; //if no enemy was found
}

void options() 
{
    cout << "_ _ _____________________________________________________________ _ _" << endl << endl;
    cout << "Commands list:" << endl <<
    "1. type 'use ___' to use an item or ability.*" << endl <<
    "2. type 'equip ___' to equip a weapon from your backpack.*" <<
    endl <<
    "3. type 'inventory' to check your backpack and equipped weapon." <<
    endl << "4. type 'health' to see your current health." <<
    endl <<
    "5. type 'inspect ___' to inspect in-game objects.**" <<
    endl << "6. type 'skip' to skip the idle phase OR move to the next level.*" << endl <<
    endl <<
    "*only ONE of these(*) actions can be taken in the idle phase" <<
    endl <<
    " and will END the idle phase if executed! Be careful!" <<
    endl << endl <<
    "** type 'inspect environment' to inspect your surroundings." <<
    endl;
    cout <<
    "_ _ _____________________________________________________________ _ _" <<
    endl;
}

void introAndInstruct(const int poisonChance, vector<string> equipped, const int fireDamage, const int knifeDamage) 
{
    cout << " _ _ _____________________________________________________________ _ _" << endl 
  << "  Welcome to 'Kobold Slayer.' A game by J.E. Hogue." << endl 
  << "  This game is similar to a dungeon crawler, and is permadeath." << endl 
  << "  You play as a specialist adventurer; skilled in dispatching kobolds." << endl 
  << "  This quest will be difficult. Prepare yourself." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl;
  
  //instructions.
  cout << " You progress through the game through the use of text commands." << endl 
  << "  The command scheme is fairly simple, and will be presented as you play the game." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  Listed below is the available items and their effects:" << endl 
  << "  'gateScroll' allows you to write a location and temporarily open a portal there:" << endl 
  << "      You can either create a portal to the ocean floor, cutting apart any one enemy with salt water;" << endl 
  << "      or you might open a portal to the nearby city to escape the kobolds." << endl 
  << "  'antidote' cures the you of poison effects. There currently is a " << (poisonChance * 10) << " percent chance that you'll be poisoned by a kobold's attack phase." << endl 
  << "  'healthPot' heals you to max health." << endl 
  << "  'stamPot' lets the player attack for double damage on their next attack phase." << endl 
  << "  'fireWater' allows the player to set all enemies in the level on fire. This ability causes enemies to experience " << fireDamage << " fire damage per turn. Crispy." << endl 
  << "  'smokeBomb' lets the player throw a smoke bomb, confusing the kobolds and denying them an attack phase." << endl 
  << "  'targe' lets the player use their shield to block all enemy attacks for one turn. Shield is destroyed if used in this way. The targe can also be used as a weapon." << endl 
  << "  'torch' is required to use the fireWater ability; and functions as a weapon. " << endl 
  << "  'dagger' is an extra equippable weapon included in your inventory, while " << equipped[0] << " is currently equipped" << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  Currently, there are several abilities available as well:" << endl 
  << "  The priestess, encountered later in the game has two miracles, with two charges left to use either:" << endl 
  << "      'heal' heals the player for 20 health." << endl 
  << "      'holyLight blinds all enemies in the room; denying them their attack phase.'" << endl 
  << "  The Kobold Slayer can use the ability 'berserk' to instantly kill all low-level enemies in the room. Useful." << endl 
  << "  You are also equipped with a number of knives. On enemy hit, these do " << knifeDamage << "damage and deny that enemy an attack phase." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  The 'bloodSoaked' debuff is incredibly important to surviving this quest." << endl 
  << "  As you dispatch enemies, your weapon becomes 'bloodStained' and eventually 'bloodSoaked.'" << endl 
  << "      'bloodStained' is placed on your currently equipped weapon when you kill your first enemy with it; with no ill effects." << endl 
  << "      'bloodSoaked' is placed on your weapon when you use it to kill its' second enemy; activating the debuff." << endl 
  << "  Once the 'bloodSoaked' debuff is added to a weapon, that weapon has a 90 percent chance of flying out of your hand on the next attack." << endl 
  << "  You lose that weapon permanently and deal 5 unarmed damage to the selected enemy. Don't test the odds." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  Rapidly switching weapons, using your items and abilities, and wise decision making will secure you in this quest." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  Inspection, as well as interaction, is another feature that requires some explanation." << endl 
  << "  All enemies and objects of interest are represented as alphanumeric characters." << endl 
  << "  When selecting an enemy to attack or an object to inspect, ALWAYS use a character." << endl 
  << "  For example; when attacking a kobold, you would type 'attack k'" << endl 
  << "  This would select the first kobold on the map in priority of top-to-bottom and left-to-right and apply damage to it." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl 
  << "  This concludes the introduction and gameplay tutorial." << endl 
  << " _ _ _____________________________________________________________ _ _" << endl << endl;
}

void idlePhase(const int& maxHealth, int level, const string level1Desc, const string level2Desc, const string level3Desc, const string level4Desc, const string level5Desc, const string level6Desc, const string level7Desc, vector<string>& backpack, bool& inspO, bool& inspKobold, bool& denyAttack, bool& denyEnemAttack, string& answer, int& health, bool& holyLight, int& miraclesLeft, int& knives, int knifeDamage, int& berserkCharges, bool& bActive, bool& gateScrollCity, bool& gateScrollOcean, bool& poisoned, bool& attackTwo, bool& onFire, bool& confused, bool& targeActive, vector<vector<string>> currentLevel, vector<string> enemyType, int armorDurability, vector<int> defaultDamage, vector<string>& equipped)  //idle phase main function
{
    for (int error = true; (error == true) && (health > 0);) //enter the idle phase. Sets error to true and exits the idle phase if error is false or the player's health is zero.
      {
        cout << "Type 'options' to see available commands." << endl; //prompts the user at the start of each idle phase to type options if unsure of a command.
        printLevel(currentLevel); //prints current level.
        getAnswer(answer); //getting new answer for the idle phase.

        if (answer == "options") {
          options();
        } else if (answer == "inventory") {
          inventory(backpack, equipped, miraclesLeft, knives, berserkCharges, level);
        } else if (answer == "health") {
          checkHealth(health, maxHealth, armorDurability);
        } else if (answer.find("inspect") != string::npos) {
          inspect(answer, currentLevel, level, level1Desc, level2Desc, level3Desc, level4Desc, level5Desc, level6Desc, level7Desc, backpack, inspO, inspKobold, denyAttack, denyEnemAttack);
        } else if (answer.find("use") != string::npos) {
          use(answer, health, maxHealth, holyLight, miraclesLeft, knives, knifeDamage, denyAttack, berserkCharges, bActive, backpack, equipped, gateScrollCity, gateScrollOcean, poisoned, attackTwo, onFire, confused, targeActive, currentLevel, enemyType, armorDurability, defaultDamage);
          if (dmgCalc(currentLevel, defaultDamage, enemyType) > 0) //if enemies do not exist, the idle loop will continue.
          {
            error = false; //ends the idle loop.
          }
        } else if (answer.find("equip") != string::npos) {
          equip(answer, backpack, equipped);
          if (dmgCalc(currentLevel, defaultDamage, enemyType) > 0) {
            error = false; //ends the idle loop.
          }
        } else if (answer.find("skip") != string::npos) {
          cout << "Skipping idle phase..." << endl;
          error = false; //ends the idle loop.

        } else {
          cout << "Command not recognized. Please try again." << endl;
        }
      } //end of idle phase.
}



