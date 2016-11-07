#include "../include/Parser.hpp"

using namespace std;


Parser::Parser(int argc, char* argv[])
{
    if (argc == 2) {
        makefile = new ifstream(argv[1], ios::in);
    } else {
        cerr << "Veuillez entrer le chemin du Makefile en argument" << endl;
        exit(1);
    }

}

int Parser::readMakefile()
{
    // TODO : passer ce test dans le constructeur ?
    if(*makefile) {
        parseMakefile();
    }
    else {
        cerr << "Erreur dans le chemin du fichier en argument" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    
}

void Parser::parseMakefile()
{
    // TODO : faire un while sur EOF et stocker cible, dependances et commande dans la structure de données qu'on va choisir
    string cible;
    vector<string> dependances;
    string commande;
    string line_tmp;
    getline( *makefile, line_tmp);
    istringstream iss(line_tmp);
    iss >> cible;

    int i = 0;
    while (iss) {
        string tmp;
        iss >> tmp;
        dependances.push_back(tmp);
        i++;
    }
    getline( *makefile, commande);
    commande = commande.substr(1); // Suppression de l'alinea

    // Verification du Makefile et correction de cible
    if (!(cible.substr(cible.length()-1).compare(":"))) {
        // cout << cible.substr(cible.length()-1) << endl;
        cible = cible.substr(0, cible.length()-1);
        cout << "Debug -> " << endl;
        cout << "Cible : " << cible << " et dépendances : ";
        for(int i=0; i<dependances.size(); i++) {
            cout << dependances[i] << " ";
        }
        cout << endl << "Commande : " << commande << endl;

    }

    makefile->close();
}

Parser::~Parser()
{

}