typedef struct maVoiture maVoiture;
struct maVoiture{
    int numero ; //numéro de la voiture
    double S1 ;
    double S2 ;  // temps des secteurs
    double S3 ;
    double tempsTotal; //temps total
    int stand ; // si présent au stand
    int out ; // 0 = abandon

};
