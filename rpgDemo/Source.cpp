//melvin su
//RPGTest1

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

struct character {
private:
	string name;
	double hp = 100.0;
	double max_hp = 100.0;
	double base_atk = 10.0;
	double base_def = 5.0;
public:
	character(string name)
	{
		this->name = name;
	}
	double getCurrentHp() { return hp; }
	double getAtk() { return base_atk; }
	double getDef() { return base_def; }
	string getName() { return name; }
	void setHp(double hp) { this->hp = hp; }
};

double damageCal(double atk, double def)
{
	double variance = ((rand() % 5) / 10.0) - 0.25;
	double mod_atk = atk + (atk * variance);
	double damage = mod_atk - def;
	//cout << "atk:" << atk << "\tvariance:" << variance << "\tmod_atk:" << mod_atk << "\tdef:" << def  << "\tfinal damage:" << damage << endl;
	if (damage > 0) {
		cout << "damage:" << damage << endl;
		return damage;
	}
	else { return 0; }
}

void combat(character p1, character p2) {
	if (p1.getCurrentHp() <= 0 || p2.getCurrentHp() <= 0) {
		return;
	}
	while (p1.getCurrentHp() > 0 && p2.getCurrentHp() > 0) {
		//p1 attacks p2
		cout << "currenthp:" << p2.getCurrentHp() << endl;
		p2.setHp(p2.getCurrentHp() - damageCal(p1.getAtk(), p2.getDef()));
		cout << p2.getName() << " current hp is " << p2.getCurrentHp() << endl;
		if (p2.getCurrentHp() >= 0) {
			p1.setHp(p1.getCurrentHp() - damageCal(p2.getAtk(), p1.getDef()));
			cout << p1.getName() << " current hp is " << p1.getCurrentHp() << endl;
		}
	}
}

int main()
{
	srand(time(NULL));

	character sheep("Sheep");
	character wolf("Wolf");
	
	combat(sheep, wolf);
	return 0;
}