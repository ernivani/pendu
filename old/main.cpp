#include <iostream>
#include <vector>

using namespace std;

template<typename T>

void afficher_v(vector<T>& v) {
	
	cout << "----------------" << endl;

	for (const T& element : v) {
		cout << element <<endl;
	}
}

void exo1() {
	vector<string> list = {"Alice","Jhon","Marc"};
	afficher_v(list);
	list.pop_back();
	afficher_v(list);	
}

void exo2() {
	vector<int> list(5);
	// list[2] = 5; 
	list.insert(list.begin()+2, 5);
	afficher_v(list);

	for (int i = 0; i < list.size(); i++) {
		for (int j = i+1; j < list.size(); j++) {
			if (list[i] < list[j]) {
				list.insert(list.begin()+i, list[j]);
				list.erase(list.begin()+j+1);
			}
		}
	}
	afficher_v(list);
			
}

void exo3() {	
	vector<int> list(5);
	int note = 0;
	for (int i = 0; i < list.size(); i++) {
		cout << "Entrer la note de l'étudiant " << i+1 << " : ";
		cin >> note;
		list[i] = note;
	}
	afficher_v(list);
	int max = list[0];
	
	for (int i = 0; i < list.size(); i++) {
		if (list[i] > max) {
			max = list[i];
		}
	}

	cout << "La note maximale est : " << max << endl;

	int min = list[0];

	for (int i = 0; i < list.size(); i++) {
		if (list[i] < min) {
			min = list[i];
		}
	}

	cout << "La note minimale est : " << min << endl;

	int sum = 0;

	for (int i = 0; i < list.size(); i++) {
		sum += list[i];
	}

	cout << "La moyenne des notes est : " << sum/list.size() << endl;


}

void exo4() {
	vector<int> list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	vector<int> list2 = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	vector<int> list3;

	for (int i = 0; i < list1.size(); i++) {
		list3.push_back(list1[i]);
	}

	for (int i = 0; i < list2.size(); i++) {
		list3.push_back(list2[i]);
	}

	for (int i = 0; i < list3.size(); i++) {
		for (int j = i+1; j < list3.size(); j++) {
			if (list3[i] == list3[j]) {
				list3.erase(list3.begin()+j);
			}
		}
	}

	afficher_v(list3);
}

void exo5() {
	vector<string> inventaire;
	string article;
	int choix = 0;

	do {
		cout << "1. Ajouter un article" << endl;
		cout << "2. Supprimer un article" << endl;
		cout << "3. Afficher l'inventaire" << endl;
		cout << "4. Quitter" << endl;
		cout << "Votre choix : ";
		cin >> choix;

		switch (choix) {
			case 1:
				cout << "Entrer l'article à ajouter : ";
				cin >> article;
				inventaire.push_back(article);
				break;
			case 2:
				cout << "Entrer l'article à supprimer : ";
				cin >> article;
				for (int i = 0; i < inventaire.size(); i++) {
					if (inventaire[i] == article) {
						inventaire.erase(inventaire.begin()+i);
					}
				}
				break;
			case 3:
				afficher_v(inventaire);
				break;
			case 4:
				break;
			default:
				cout << "Choix invalide" << endl;
				break;
		}
	} while (choix != 4);
}

void exo6() {
	vector<vector<int>> ventes(3, vector<int>(7));
	int vente = 0;
	int total = 0;

	srand(time(0));
	for (int i = 0; i < ventes.size(); i++) {
		for (int j = 0; j < ventes[i].size(); j++) {
			ventes[i][j] = rand() % 100;
		}
	}
	for (int i = 0; i < ventes.size(); i++) {
		total = 0;
		for (int j = 0; j < ventes[i].size(); j++) {
			total += ventes[i][j];
		}
		cout << "Le total des ventes du produit " << i+1 << " est : " << total << endl;
	}
	
}

void exo7() {
	vector<int> list = {1, 2, 3, 4, 5};
	int temp = 0;
	for (int i = 0; i < 3; i++) {
		temp = list[list.size()-1];
		for (int j = list.size()-1; j > 0; j--) {
			list[j] = list[j-1];
		}
		list[0] = temp;
		afficher_v(list);
	}

}

void exo8() {
	vector<int> places(10, 1);
	int choix = 0;
	int place = 0;

	do {
		cout << "1. Réserver une place" << endl;
		cout << "2. Annuler une réservation" << endl;
		cout << "3. Afficher les places" << endl;
		cout << "4. Quitter" << endl;
		cout << "Votre choix : ";
		cin >> choix;

		switch (choix) {
			case 1:
				cout << "Entrer le numéro de la place à réserver : ";
				cin >> place;
				if (places[place-1] == 1) {
					places[place-1] = 0;
				} else {
					cout << "La place est déjà occupée" << endl;
				}
				break;
			case 2:
				cout << "Entrer le numéro de la place à annuler : ";
				cin >> place;
				if (places[place-1] == 0) {
					places[place-1] = 1;
				} else {
					cout << "La place est déjà disponible" << endl;
				}
				break;
			case 3:
				afficher_v(places);
				break;
			case 4:
				break;
			default:
				cout << "Choix invalide" << endl;
				break;
		}
	} while (choix != 4);
}

int main() {
	// exo1();
	// exo2();
	// exo3();
	// exo4();
	// exo5();
	// exo6();
	// exo7();
	// exo8();	

	return 0;
}
