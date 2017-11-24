#include<iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  // static used for efficiency, so we only calculate this value once
	// evenly distribute the random number across our range
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature {
protected:
	std::string name;
	char symbol;
	int health;
	int demage;
	int gold;
public:
	Creature(std::string name, char symbol, int health, int demage, int gold) : name(name), symbol(symbol),health(health), demage(demage), gold(gold) {}
	std::string getName() { return name; }
	char getSymbol() const { return symbol; }
	int getHealth() const { return health; }
	int getDemage() const { return demage; }
	int getGold() const { return gold; }

	void reduceHealth(int h) { health -= h; }
	bool isDead() { return (health <= 0); }
	void addGold(int g) { gold += g; }
};

class Player : public Creature {
	int level;
public:
	Player(std::string name) : Creature(name, '@', 10, 1, 0), level(1) {}
	void levelUp() { ++level; ++demage; }
	int getLevel() const { return level; }
	bool hasWon() const { return (level >= 20);}
};

class Monster : public Creature {
public:
	enum Type { DRAGON, ORC, SLIME, MAX_TYPES };
	struct MonsterData {
		std::string name;
		char symbol;
		int health;
		int damage;
		int gold;
	};
	Monster(Type type) : Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold) {}
	static MonsterData monsterData[MAX_TYPES];
	static Monster getRandomMonster() {
		int i =	getRandomNumber(0,MAX_TYPES - 1);

		return Monster(static_cast<Type>(i));
	}
};


Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};


void attackMonster(Player& p, Monster& m) {
	std::cout << "You hit the " << m.getName() << " for " << p.getDemage() <<" damage." << std::endl;
	m.reduceHealth(p.getDemage());
}

void attackPlayer(Player& p, Monster& m) {
	std::cout << "The " << m.getName() <<  " hit you for " << m.getDemage() <<" damage." << std::endl;
	p.reduceHealth(m.getDemage());
}


void fightMonster(Player& p, Monster& m) {
	char key;
	while ( !p.isDead() && !m.isDead() ) {
		std::cout << "(R)un or (F)ight: ";
		std::cin >> key;
		if ( key == 'f' || key == 'F' ) {
			attackMonster(p, m);
			if ( m.isDead() ) {
				std::cout << "You killed the " << m.getName() << "." << std::endl;
				p.addGold(m.getGold());
				p.levelUp();
				std::cout << "You are now level " << p.getLevel() << "." << std::endl;
				std::cout << "You found " << m.getGold() << " gold." << std::endl;
			} else {
				attackPlayer(p, m);
			}
		} else 	if ( key == 'r' || key == 'R' ) {
			int chance = getRandomNumber(0, 100);
			if ( chance < 50 ) {
				std::cout << "You failed to flee." << std::endl;
				attackPlayer(p, m);
			} else {
				std::cout << "You successfully fled." << std::endl;
				return;
			}
	}

	}
}




int main() {
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // get rid of first result


	std::string name;
	std::cout << "Enter your name: ";
	std::cin >> name;
	Player p(name);
	std::cout << "Welcome, " << p.getName() << "." << std::endl;
	while( !p.hasWon() && !p.isDead() ) {
		Monster m = Monster::getRandomMonster();
		std::cout << "You have encountered a " << m.getName() << " (" << m.getSymbol() << ").\n";
		fightMonster(p, m);
	}
	if ( p.isDead() ) {
		std::cout << "You died at level " << p.getLevel() << " and with " << p.getGold() << " gold.\n";
		if ( p.getGold() > 0 ) {
			std::cout << "Too bad you can't take it with you! \n";
		}
	} else if ( p.hasWon() ) {
		std::cout << "You won the game with " << p.getGold() << " gold!!!!!\n";
	}

return 0;
}
