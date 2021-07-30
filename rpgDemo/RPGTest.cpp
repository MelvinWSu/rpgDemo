//melvin su
//RPGTest

#include <iostream>
#include <random>
#include <ctime>
#include <iomanip> 

#include <vector>
#include <algorithm>

using namespace std;

//damage variance is between variance_percentage and -(variance_percentage)
double variance_percentage = 0.25;
uniform_real_distribution<double> skill_selection(-variance_percentage, variance_percentage);
uniform_real_distribution<double> choice(0, 3);

struct effect_template {
private:
	string name;
public:
	effect_template() { name = "No Name"; }
	effect_template(string name) { this->name = name; }
	string getName() { return name; }
	void setName(string name) { this->name = name; }
};

//statuses to implement: 1.Burn, after battle - deal 5% (max?) HP per burn status, last at most 3 turns, clear all burn statuses when oldest one depletes
//2. Frost, calculation - reduce atk/mag by %50, last 1 turn
//3. Shock, calculation - reduce DEF by %25, increase MAG by %25, infinite duration
//4. Guard, calculation - reduce final damage from attacks by 90%, last 1 turn
//5. Protection, calculation - reduce final damage from attacks and skills by 10%, prevent negative status, last 3 turns

//before/after battle phase
//have battle phase code/ damage calculation code check for needed effects
//Burn-specific: burn status can be applied multiple times for stacking damage, all burn statuses removed once oldest one's duration changes to 0

struct status : public effect_template {
private:
	int duration = 0;
public:
	status() { setName("No Status Name"), duration = 99; }
	status(string name, int duration = 99) { setName(name); this->duration = duration; }
	void effect() {};
	int getDuration() { return duration; }
	void decrementDuration() { --duration; };
};

status empty_st("No Status Name",99), burn_st("Burn", 3), frost_st("Frost", 1), shock_st("Shock", 99), guard_st("Guard", 1), protection_st("Protection", 3);
vector <status> global_status_list = { burn_st, frost_st, shock_st, guard_st, protection_st };

//targeting: 0 = self, 1 = other

struct skill : public effect_template {
private:
	pair<double, int> skill_target;
	vector <pair<status, int>> status_targets;
public:
	skill(string name) : effect_template(name) {}
	skill(string name, pair<double, int> skill_target) { setName(name); this->skill_target = skill_target; }
	skill(string name, vector <pair<status, int>> status_targets) { setName(name); this->status_targets = status_targets; }
	skill(string name, pair<double, int> skill_target, vector <pair<status, int>> status_targets) 
	{
		setName(name); this->skill_target = skill_target; this->status_targets = status_targets; 
	}
	
	double getSkillTarget_damage() { return skill_target.first; }
	int getSkillTarget_target() { return skill_target.second; }
	vector <pair<status, int>> getStatusTargets() { return status_targets; }
};

//skills to implement: 1. Magic Shot
//2. Flame Blast
//3. Frost Spines
//4. Shockwave
//5. Guard
//6. Heal Up
//7. Protection
//8. ATK UP
//9. DEF UP

skill magic_shot_sk("Magic Shot", {1.5, 1}),
flame_blast_sk("Flame Blast", { 0.75, 1 }, vector<pair<status, int>>(1, make_pair(burn_st, 1))),
frost_spines_sk("Frost Spines", { 1.0, 1 }, vector<pair<status, int>>(1, make_pair(frost_st, 1))),
shockwave_sk("Shockwave", { 1.0, 1 }, vector<pair<status, int>>(1, make_pair(shock_st, 1))),
guard_sk("Guard", vector<pair<status, int>>(1, make_pair(guard_st, 0))),
heal_up_sk("Heal Up", { 1, 0 }),
protection_sk("Protection", vector<pair<status, int>>(1, make_pair(protection_st, 0))),
atk_up_sk("ATK Up"),
def_up_sk("DEF Up");

//vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk, frost_spines_sk, shockwave_sk, guard_sk, heal_up_sk, protection_sk, atk_up_sk, def_up_sk };
//vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk, frost_spines_sk, shockwave_sk, guard_sk, heal_up_sk, protection_sk };
vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk };
uniform_int_distribution<int> skill_num_distributor(0, global_skill_list.size() - 1);

struct character {
private:
	string name;
	double hp = 100.0;
	double max_hp = 100.0;
	double base_atk = 12.0;
	double base_def = 5.0;

	double mp = 50;
	double max_mp = 50;
	double base_mag = 5;

	vector <skill> skill_list;
	int skill_count_limit = 3;
	vector <status> status_list;
public:
	character(string name) { this->name = name; }
	double getCurrentHp() { return hp; }
	double getAtk() { return base_atk; }
	double getDef() { return base_def; }
	string getName() { return name; }
	void setHp(double hp) { this->hp = hp; }

	double getMag() { return base_mag; }
	double getMaxHP() { return max_hp; }
	double getMp() { return mp; }
	double getMaxMp() { return max_mp; }
	void addSkill(skill s) { skill_list.push_back(s); }
	vector <skill> getSkills() { return skill_list; }
	void distribute_skills()
	{
		while (skill_list.size() < 3)
		{
			random_device r;
			default_random_engine re(r());;
			int iter = skill_num_distributor(re);
			skill_list.push_back(global_skill_list[iter]);
		}
	}
	void printSkills()
	{
		for (int i = 0; i < skill_list.size(); i++)
		{
			cout << skill_list[i].getName() << endl;
		}
	}
	void addStatus(status s) { status_list.push_back(s); }
	vector <status> getStatuses() { return status_list; }
	void decrementAllStatus()
	{
		if (status_list.size() != 0)
		{
			for (int i = 0; i < status_list.size(); i++)
			{
				if (status_list[i].getDuration() <= 1)
				{
					if (status_list[i].getName() == "Burn")
					{
						for (int j = 0; j < status_list.size(); j++)
						{
							if (status_list[j].getName() == "Burn")
							{
								status_list.erase(status_list.begin() + j);
							}
						}
					}
					else
					{
						status_list.erase(status_list.begin() + i);
					}
					
				}
				else { status_list[i].decrementDuration(); }
			}
		}
	}
	void printStatuses()
	{
		if (status_list.size() != 0) 
		{
			cout << "----" << getName() << "'s Statuses----" << endl;
			for (int i = 0; i < status_list.size(); i++)
			{
				cout << status_list[i].getName() << ", " << status_list[i].getDuration() << " turns remaining" << endl;
			}
			cout << endl;
		}
	}
};

//order of bits
//user frost, user shock, target shock, target guard, target protection
double attackCal(double atk, double def = 0, unsigned char c = 0)
{
	random_device r;
	default_random_engine re(r());

	double frost_calc = 0, shock_u_calc = 0, shock_t_calc = 0, guard_calc = 0, protection_calc = 0;
	

	double variance = skill_selection(re) + 1.0;
	double mod_atk = atk * variance;
	if (c & (1 << 0)) { frost_calc = mod_atk * 0.5; }
	//if (c & (1 << 1)) { shock_u_calc = 1; }
	if (c & (1 << 2)) { shock_t_calc = def * 0.75; }
	
	double damage = (mod_atk - frost_calc) - (def - shock_t_calc);

	if (c & (1 << 3)) { damage *= 0.1; }
	if (c & (1 << 4)) { damage *= 0.9; }
	
	//cout << "atk:" << atk << "\tvariance:" << variance << "\tmod_atk:" << mod_atk << "\tdef:" << def  << "\tfinal damage:" << damage << endl;
	//cout << "attack damage" << endl;
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

double magCal(double mag1, double skill_power, double mag2 = 0, double def2 = 0, unsigned char c = 0)
{
	double frost_calc = 0, shock_u_calc = 0, shock_t_calc = 0, guard_calc = 0, protection_calc = 0;
	if (c & (1 << 0)) { frost_calc = mag1 * 0.5; }
	if (c & (1 << 1)) { shock_u_calc = mag1 * 1.25; }
	//if (c & (1 << 2)) { shock_t_calc = def2 * 0.75; }
	//if (c & (1 << 3)) { guard_calc = 1; }

	double damage = ((mag1 - frost_calc + shock_u_calc) * skill_power) - (mag2/2);
	if (c & (1 << 4)) { damage *= 0.9; }
	//cout << "magic damage:" << damage << endl;
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

void turn(character *p1, character *p2)
{
	random_device r;
	default_random_engine re(r());
	vector<status> temp_status1 = p1->getStatuses();
	vector<status> temp_status2 = p2->getStatuses();
	int use = choice(re);
	double damage;
	p1->printStatuses();
	//use indicates if attack or skill is selected
	if (use == 0) {
		cout << p1->getName() << " attacks " << endl;
		damage = attackCal(p1->getAtk(), p2->getDef());
		p2->setHp(p2->getCurrentHp() - damage);
		cout << p1->getName() << " hits for " << damage << endl << p2->getName() << " current hp is " << p2->getCurrentHp() << endl;
	}
	else {
		int target = p1->getSkills()[use - 1].getSkillTarget_target();
		double power = p1->getSkills()[use - 1].getSkillTarget_damage();
		string name = p1->getSkills()[use - 1].getName();

		//target indicates if spell selected is damaging to opponent or helpful to user
		cout << p1->getName() << " used " << name << endl;
		if (target == 1) {
			damage = magCal(p1->getMag(), power, p2->getMag(), p2->getDef());
			p2->setHp(p2->getCurrentHp() - damage);
			cout << p1->getName() << " hits for " << damage << endl << p2->getName() << " current hp is " << p2->getCurrentHp() << endl;
		}
		else if (target == 0 && power != 0) {
			damage = magCal(p1->getMag(), power);
			if (damage + p1->getCurrentHp() > p1->getMaxHP()) {
				damage = p1->getMaxHP() - p1->getCurrentHp();
			}
			p1->setHp(p1->getCurrentHp() + damage);
			cout << p1->getName() << " has healed by " << damage << " to " << p1->getCurrentHp() << endl;
		}
		vector <pair<status, int>> temp = p1->getSkills()[use - 1].getStatusTargets();
		for (int i = 0; i < temp.size(); i++)
		{
			if (get<1>(temp[i]) == 1)
			{
				p2->addStatus(get<0>(temp[i]));
				cout << p2->getName() << " has been inflicted with " << get<0>(temp[i]).getName() << endl;
			}
			else
			{
				p1->addStatus(get<0>(temp[i]));
				cout << p1->getName() << " has received " << get<0>(temp[i]).getName() << endl;
			}
		}
	}
	if (p2->getCurrentHp() <= 0) {
		cout << p2->getName() << " is defeated!" << endl;
		return;
	}

	int burn_count = 0;
	for (int i = 0; i < temp_status1.size(); i++)
	{
		if (temp_status1[i].getName() == "Burn")
		{
			++burn_count;
		}
	}

	if (burn_count)
	{
		double temp_health = p1->getCurrentHp();
		double burn_damage = temp_health * 5 * burn_count / 100.00;
		temp_health -= burn_damage;
		p1->setHp(temp_health);
		cout << p1->getName() << " has been Burned for " << burn_damage << " to " << temp_health << endl;
	}
	p1->decrementAllStatus();
}

void battle_phase(character *p1, character *p2) {
	
	while (p1->getCurrentHp() > 0 && p2->getCurrentHp() > 0)
	{
		turn(p1, p2);
		if (p1->getCurrentHp() <= 0 && p2->getCurrentHp() <= 0)
		{
			return;
		}

		turn(p2, p1);
		if (p1->getCurrentHp() <= 0 && p2->getCurrentHp() <= 0)
		{
			return;
		}
		cout << endl;
		cin.ignore();
	}

}

int main()
{
	srand(time(NULL));

	character person1("Warrior");
	character person2("Soldier");
	
	person1.distribute_skills();
	person2.distribute_skills();

	cout << "=====" << person1.getName() << "'s skills=====" << endl;
	person1.printSkills();
	cout << "=====" << person2.getName() << "'s skills=====" << endl;
	person2.printSkills();
	cout << endl;

	battle_phase(&person1, &person2);
	return 0;
}