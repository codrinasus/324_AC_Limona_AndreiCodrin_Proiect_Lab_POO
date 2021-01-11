#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

class Copil 
{
private:
	string nume;
	string prenume;
	string varsta;
	string oras;
	string gen;
public:
	Copil(string nume, string prenume, string varsta, string gen, string oras);
	friend class Elfi;
};

Copil::Copil(string nume,string prenume,string varsta,string gen,string oras)
{
	this->nume = nume;
	this->prenume = prenume;
	this->varsta = varsta;
	this->gen = gen;
	this->oras = oras;
}

class Wishlist 
{
private:
	vector <string> gifts;
public:
	Wishlist(vector <string> gifts);
	friend class Elfi;
};

Wishlist::Wishlist(vector <string> gifts)
{
	this->gifts = gifts;
}

class Elfi
{
private:
	static int acadele;
	static int ambalaje_fete;
	static int ambalaje_baieti;
	static int bad_kids;
	vector <Copil> kids;
	vector <Wishlist> list;
	vector <string> status;
	vector <string> shop;
	vector <int> price;
	vector <string> lista_orase;
	vector <float> distante;
public:
	void distribuireCadouri();
	int getAcadele();
	int getAmbalajFete();
	int getAmbalajBaieti();
	int getBadKids();
	vector <float> getDistante();
	vector <string> getListaOrase();
	Elfi();
	~Elfi();
};

int Elfi::acadele = 0;
int Elfi::ambalaje_fete = 0;
int Elfi::ambalaje_baieti = 0;
int Elfi::bad_kids = 0;

Elfi::Elfi()
{
	float dist;
	string aux;
	string nume, prenume, varsta, gen, oras,err;
	vector <string> wish;
	int i;
	try
	{
		//Citire date despre copii 
		ifstream kids_db;
		kids_db.open("Children.csv");
		if (!kids_db.is_open())
		{
			err = "Fisierul Children.csv nu a putut fi deschis\n";
			throw err;
		}
		while (getline(kids_db, aux))
		{
			istringstream input(aux);
			getline(input, nume, ',');
			getline(input, prenume, ',');
			getline(input, varsta, ',');
			getline(input, gen, ',');
			getline(input, oras);
			kids.push_back(Copil(nume, prenume, varsta, gen, oras));
		}
		kids_db.close();

		//Citire cadouri de pe listele copiilor
		ifstream list_db;
		list_db.open("Wishlist.csv");
		if (!list_db.is_open())
		{
			err = "Fisierul Whislist.csv nu a putut fi deschis\n";
			throw err;
		}
		while (getline(list_db, aux))
		{
			istringstream input(aux);
			while (getline(input, aux, ','))
				wish.push_back(aux);
			list.push_back(Wishlist(wish));
			wish.clear();
		}
		list_db.close();


		//citire cadouri din atelier
		ifstream shop_db;
		shop_db.open("Workshop.csv");
		if (!shop_db.is_open())
		{
			err = "Fisierul Workshop.csv nu a putut fi deschis\n";
			throw err;
		}
		while (getline(shop_db, aux))
		{
			istringstream input(aux);
			getline(input, aux, ',');
			shop.push_back(aux);
			getline(input, aux);
			price.push_back(stoi(aux));
		}
		shop_db.close();

		//citire status copil
		ifstream status_db;
		status_db.open("Status.csv");
		if (!status_db.is_open())
		{
			err = "Fisierul Status.csv nu a putut fi deschis\n";
			throw err;
		}
		while (getline(status_db, aux))
			status.push_back(aux);
		status_db.close();

		//citire orase si distante
		ifstream drum_db;
		drum_db.open("Drum.csv");
		if (!drum_db.is_open())
		{
			err = "Fisierul Drum.csv nu a putut fi deschis\n";
			throw err;
		}
		getline(drum_db, aux);
		istringstream input(aux);
		while (getline(input, aux, ','))
			lista_orase.push_back(aux);

		for (i = 0; i < (lista_orase.size() * (lista_orase.size() - 1)) / 2; i++)
		{
			drum_db >> dist;
			distante.push_back(dist);
		}
		drum_db.close();
	}
	catch (string err)
	{
		cout << err;
		throw;
	}
}

void Elfi::distribuireCadouri()
{
	int suma,i,k;
	bool ok;
	vector <string> aux;
	for (i = 0; i < status.size(); i++)
	{
		//atribuire buget in functie de statusul copilului
		ok = false;
		if(status[i]=="Good")
			suma = 100;
		else
		{
			suma = 10;
			bad_kids++;
		}
		//incrementare tip ambalaj folosit
		if (kids[i].gen == "M")
			ambalaje_baieti++;
		else
			ambalaje_fete++;
		//cautare cadouri in atelier in functie de buget
		for (string j : list[i].gifts)
		{
			for (k = 0; k < shop.size(); k++)
			{
				if ((shop[k] == j)&&(suma >= price[k]))
				{
					ok = true;
					suma -= price[k];
					aux.push_back(shop[k]);
				}
			}
		}
		//Afisare cadouri default
		if (!ok)
		{
			if (suma == 100)
			{
				cout << kids[i].nume << " " << kids[i].prenume << ", varsta " << kids[i].varsta << " ani, din orasul " << kids[i].oras <<", a primit: Culegere POO\n";

			}
			if (suma == 10)
			{ 
				cout << kids[i].nume << " " << kids[i].prenume << ", varsta " << kids[i].varsta << " ani, din orasul " << kids[i].oras << ", a primit: Caciula GitHub, carbune\n";
			}
		}
		//Afisare cadouri primite
		else
		{
			acadele += suma;
			cout << kids[i].nume << " " << kids[i].prenume << ", varsta " << kids[i].varsta << " ani, din orasul " << kids[i].oras << ", a primit:";
			cout << " " << aux[0];
			for (k = 1; k < aux.size(); k++)
			{
				cout << ", " << aux[k];
			}

			if (suma != 0)
				cout << ", " << suma << " acadele";
			if (status[i] == "Bad")
				cout << ", carbune";

			cout << "\n";
			aux.clear();
		}
	}
	cout << "\n";
}

int Elfi::getAcadele()
{
	return acadele;
}

int Elfi::getAmbalajBaieti()
{
	return ambalaje_baieti;

}

int Elfi::getAmbalajFete()
{
	return ambalaje_fete;
}

int Elfi::getBadKids()
{
	return bad_kids;
}

vector <float> Elfi::getDistante()
{
	return distante;
}

vector <string> Elfi::getListaOrase()
{
	return lista_orase;

}

Elfi::~Elfi()
{
	kids.clear();
	list.clear();
	status.clear();
	shop.clear();
	price.clear();
	lista_orase.clear();
	distante.clear();
}

class Troli
{
private:
	int ambalaje_baieti;
	int ambalaje_fete;
	int bad_kids;
public:
	Troli (int ambalaje_baieti,int amabalaje_fete,int bad_kids);
	void ambalajeFolosite();
	int getBadKids();
};

Troli::Troli(int ambalaje_baieti, int ambalaje_fete,int bad_kids)
{
	this->ambalaje_baieti = ambalaje_baieti;
	this->ambalaje_fete = ambalaje_fete;
	this->bad_kids = bad_kids;
}

void Troli::ambalajeFolosite()
{
	cout << "Au fost folosite " << ambalaje_baieti << " ambalaje de baieti si " << ambalaje_fete << " ambalaje de fete\n\n";
}

int Troli::getBadKids()
{
	return bad_kids;
}

class MosCraciun
{
private:
	float dist[50][50];
	vector <string> orase;
	vector <string> drum;
	vector <string> drum_min;
	float total_dist;
	vector <bool> vizitat;
public:
	void santaRoad(int pos_curent,int count,float cost);
	float getTotalDist();
	vector <string> getDrumMin();
	MosCraciun(vector <float> distante, vector <string> orase);
	~MosCraciun();
};

MosCraciun::MosCraciun(vector <float> distante, vector <string> orase)
{
	this->orase = orase;
	int i,j,k=0;
	//Se construieste graful ponderat
	for (i = 0; i < orase.size() - 1; i++)
	{
		for (j = i + 1; j < orase.size(); j++)
		{

			dist[i][j] = distante[k];
			dist[j][i] = distante[k];
			k++;
		}
		dist[i][i] = 0;
	}
	dist[i][i] = 0;

	//se construieste un vector de vizitare
	for (i = 0; i < orase.size(); i++)
		vizitat.push_back(false);
	vizitat[0] = true;
	//se initializeaza drumul 
	drum.push_back(orase[0]);
	total_dist = 999999;
}

//calcularea drum critic folosind backtracking
void MosCraciun::santaRoad(int pos_curent, int count, float cost)
{
	
	if (count == orase.size())
	{
		if (total_dist > cost)
		{
			total_dist = cost;
			drum_min = drum;
		}
		return;
	}

	for (int i = 0; i < orase.size(); i++) 
	{
		if (!vizitat[i] && dist[pos_curent][i])
		{

			vizitat[i] = true;
			drum.push_back(orase[i]);
			santaRoad(i ,count + 1 ,cost + dist[pos_curent][i]);

			drum.pop_back();
			vizitat[i] = false;
		}
	}
}

float MosCraciun::getTotalDist()
{
	return total_dist;
}

vector <string> MosCraciun::getDrumMin()
{
	return drum_min;
}

MosCraciun::~MosCraciun()
{
	orase.clear();
}

class DoamnaCraciun 
{
private:
	int nr_carbuni;
	int nr_acadele;
public:
	DoamnaCraciun(int nr_carbuni, int nr_acadele);
	void costuriSuplimentare();
	template <typename T> 
	void afisare_drum(T x);
};

template <typename T>
void DoamnaCraciun::afisare_drum(T x)
{
	cout << "Km parcursi: " << x << "\n";
}

template <>
void DoamnaCraciun::afisare_drum(vector <string> x)
{
	cout << "Drumul parcurs: ";
	for (string i : x)
		cout << i << "   ";
	cout << "\n";
}

DoamnaCraciun::DoamnaCraciun(int nr_carbuni, int nr_acadele)
{
	this->nr_carbuni = nr_carbuni;
	this->nr_acadele = nr_acadele;
}

void DoamnaCraciun::costuriSuplimentare()
{
	float suma;
	suma = nr_acadele + 0.5 * nr_carbuni;
	cout << "Costul suplimentar de acadele si carbuni este de " << suma << "$\n\n";
}

class Worker 
{
protected:
	static Elfi Legolas;
public:
	virtual void work()=0;
};
Elfi Worker::Legolas;

class SoloWorker:public Worker
{
public:
	void work();
};

void SoloWorker::work()
{
	Legolas.distribuireCadouri();
}

class TeamWorker:public Worker
{
public:
	void work();
};

void TeamWorker::work()
{
	Troli RandomNoob1337(Legolas.getAmbalajBaieti(), Legolas.getAmbalajFete(), Legolas.getBadKids());
	RandomNoob1337.ambalajeFolosite();

	MosCraciun AladeenShabazz(Legolas.getDistante(), Legolas.getListaOrase());
	AladeenShabazz.santaRoad(0, 1, 0);

	DoamnaCraciun SecretIdentity(RandomNoob1337.getBadKids(), Legolas.getAcadele());
	SecretIdentity.costuriSuplimentare();
	SecretIdentity.afisare_drum<float>(AladeenShabazz.getTotalDist());
	SecretIdentity.afisare_drum<vector <string> >(AladeenShabazz.getDrumMin());
}


int main()
{
	try
	{
		SoloWorker hardworker;
		TeamWorker smartworker;
		hardworker.work();
		smartworker.work();
	}
	catch (...)
	{
		return 1;
	}
	return 0;
}