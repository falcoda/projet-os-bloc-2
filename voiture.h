typedef struct maVoiture maVoiture;
struct maVoiture{
    int numero ; //numéro de la voiture
    double S1 ;
    double S2 ;  // temps des secteurs
    double S3 ;
    double tempsTotal; //temps depuis le quel les voitures roules
    int stand ; // si présent au stand
    int out ; // 1 = crashé
    double meilleurTemps;  // meilleur temps de tour

};
