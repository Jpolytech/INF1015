#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
using gsl::span;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {

	Film& operator[](int index) const
	{
		return *elements[index];
	}

public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	Acteur* trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	
	Film* chercherFIlm(const auto& critere)
	{
		for (auto&& film; enSpan())
			if (critere(film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};


class ListeActeurs
{
	ListeActeurs(int taille);
	ListeActeurs() = default;
	span<Acteur*> spanListeActeurs() const;
	void setElements(int taille);
	void setListe(unique_ptr<Acteur* []> nouvelleListe);
	void setCapacity(int taille);
	int getElements();
	int getCapacity();
	int capacite, nElements;
	unique_ptr<Acteur* []> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

class Film
{
public:

	//constructeur
	Film(const string& titre, const string& realisateur, int anneeSortie, double recette, ListeActeurs acteurs);

	//destructeur
	~Film();

	//accesseurs
	string getTitre() const;
	string getRealisateur() const;
	int getAnneeSortie() const;
	double getRecette();
	ListeActeurs getActeurs() const;

	//surchage de l'opérateur
	friend ostream& operator<<(ostream& os, const Film& film);

	//Deepcopy des films
	Film() = default;
	Film(Film& other)
	{
		*this = other;
	}
	Film& operator=(Film & other)
	{
		if (&other == this)
			return *this;
		titre = other.titre;
		realisateur = other.realisateur;
		anneeSortie = other.anneeSortie;
		recette = other.recette;
		for (int i = 0; i < other.acteurs.getCapacity(); i++)
			acteurs.elements[i] = other.acteurs.elements[i];
		return *this;
	}

private:
	string titre;
	string realisateur;
	int anneeSortie;
	double recette;
	ListeActeurs acteurs;


	/*std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;*/
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
