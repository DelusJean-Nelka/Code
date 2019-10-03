/* Nom du fichier   : fonctions.cpp
   Auteur           : PAZIMNA Solibia
   Date de creation : 31-03-2019
   Objet            : Definition des fonctions
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <ctime>


using namespace std;
using namespace cv;

#define NIVEAU_PIXEL_TOTAL 256

//Fonction d'affichage du message d'accueil
string getIDNomImage()
/*
    Objet           : Fonction permettant de générer un identifiant
                        aléatoire a partir de l'heure système
    Type de retour  : string
*/
{
    time_t seconds;
    time(&seconds);
    stringstream ss;
    ss << seconds;
    string ts = ss.str();
    return "_"+ts;
}//Fin de la fonction getIDNomImage



int getMaxiValue(int data[], int N)
/*
    Objet           : Fonction permettant de trouver la valeur maximal dans une matrice d'entier
    Type de retour  : int
*/
{
    int data_element = 0;
    int max = 0;
    for (int i = 0; i < N; ++i)
    {
        data_element = data[i];
        if (max < data_element)
            max = data_element;
    }
    return max;
}//Fin de la fonction getMaxiValue



Scalar getScalar(int opt)
/*
    Objet           : Fonction permettant de trouver un scalar pour l'initialisation d'un cv::MAT créer
                        selon un parametre indiquant d'un option le scalar si c'est R G ou B
    Type de retour  : Scalar
*/
{
    switch(opt)
    {
    case 0:
        return Scalar(255, 0, 0);
    case 1:
        return Scalar(0, 255, 0);
    case 2:
        return Scalar(0, 0, 255);
    }
    return  Scalar(255, 0, 0);
}//Fin de la fonction getScalar


int getExtremum(uchar **data, int M, int N, int maxOrMin)
/*
    Objet           : Fonction permettant de trouver un extrémum (minimum ou maximum dans une matrice
    Type de retour  : int
*/
{
    int resultMaxOrMin = -1;
    uchar data_element = 0;

    if (maxOrMin==0) //Si c'est minimum il faut trouver
    {
        resultMaxOrMin = 255;
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < N;  ++j)
            {
                data_element = data[i][j];
                if (resultMaxOrMin > data_element)
                {
                    resultMaxOrMin = data_element;
                }
            }
        }
    }
    else if (maxOrMin==1)  //Si c'est maximum il faut trouver
    {
        int resultMaxOrMin = 0;
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < N;  ++j)
            {
                data_element = data[i][j];
                if (resultMaxOrMin < data_element)
                {
                    resultMaxOrMin = data_element;
                }
            }
        }
    }
    return resultMaxOrMin;
}//Fin de la fonction getExtremum


int calculGama(int valeur, float gama)
/*
    Objet           : Fonction permettant de calculer
                      la nouvel valeur d'une pixel en fonction de la valeur de gama
    Type de retour  : int
*/
{
    float vl = (float) valeur;
    float res = (float)pow(vl / 255.0, gama);
    return (int) (255.0 * res);
}//Fin de la fonction calculGama


vector<string> getImageFolder(string s)
/*
    Objet           : Fonction permettant de récuperer le repertoire courant de l'image et le nom de l'image dans un vecteur
                      la nouvel valeur d'une pixel en fonction de la valeur de gama
    Type de retour  : vector<string>
*/
{
    string delimiter = "/";
    string rep = "";
    vector<string> repVect;

    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    for (int i=0; i<res.size()-1; i++)
    {
        rep.append(res.at(i)+"/");
    }
    repVect.push_back(rep);
    repVect.push_back(res.at(res.size()-1));
    return repVect;
}//Fin de la fonction getImageFolder


void writeImage(Mat histo, Mat imgContast, Mat histoConstrast, string paf)
/*
    Objet           : Fonction permettant d'écrire une image dans un fichier
    Type de retour  : void
*/
{
    vector<string> repVect = getImageFolder(paf);
    imwrite(repVect.at(0)+repVect.at(1)+"_"+"histo"+getIDNomImage()+".tif", histo);
    imwrite(repVect.at(0)+repVect.at(1)+"_"+"cont"+getIDNomImage()+".tif", imgContast);
    imwrite(repVect.at(0)+repVect.at(1)+"cont_histo"+getIDNomImage()+".tif", histoConstrast);
    cout<<"Resultats stockés dans dans le repertoire de l'image originale ("+repVect.at(0)+")\n";
}//Fin de la fonction writeImage



void tracerHistogramme(Mat actImg, Scalar scalar, Mat hist, int color)
/*
    Objet           : Fonction permettant de tracer l'histogramme d'une image
    Type de retour  : void
*/
{
    int hist_h = hist.rows/3;

    int data[NIVEAU_PIXEL_TOTAL];//tableau de niveau 256
    Point point_start, point_end;
    for (int i = 0; i < NIVEAU_PIXEL_TOTAL; i++)//initialisation du tableau a 0
        data[i] = 0;

    int data_index = 0;
    int img_w = actImg.cols;
    int img_h = actImg.rows;
    for(int i = 0; i < img_h; i++)//Pour chaque pixel de l'image, on trace cherche sa hauteur
    {
        for (int j = 0; j < img_w; ++j)
        {
            data_index = actImg.at<uchar>(i,j);
            data[data_index]++;
        }
    }
    int max = getMaxiValue(data, NIVEAU_PIXEL_TOTAL);//Recherche du nombre de pixel le plus grand
    for (int i = 1; i < NIVEAU_PIXEL_TOTAL; i++)//Pour chaque nombre de pixel récupérer dans le tableau data, on trace l'histogramme
    {
        point_start = Point(i, color*hist_h);
        point_end = Point(i, color*hist_h - hist_h*data[i]/max);
        line( hist, point_start, point_end, scalar, 1, 8, 0 );
    }
}//Fin de la fonction tracerHistogramme



Mat getcontrastLinaireSaturation(Mat actImg,int s_min, int s_max)
/*
    Objet           : Fonction permettant de calculer le contraste linéaire par saturation d'une image
    Type de retour  : cv::Mat
*/
{

    int img_h = actImg.rows;
    int img_w = actImg.cols;
    int lut[NIVEAU_PIXEL_TOTAL];//Création d'une look up table
    uchar **I;
    I = new uchar*[img_h];
    for (int i = 0; i < img_h; ++i)
        I[i] = new uchar[img_w];

    int _max = s_max;
    int _min = s_min;
    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            I[i][j] = actImg.at<uchar>(i, j);
        }

    for (int i = 0; i <= NIVEAU_PIXEL_TOTAL; ++i)
        lut[i] = 255*(i - _min)/(_max - _min); //Calcul des valeurs des nouvelles pixels pour les stocker dans la lut afin de ne pas refaire les calculs
    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            if (lut[I[i][j]] < 0)
                actImg.at<uchar>(i, j) = 0;//Si une valeur de pixel est inférieur a 0 on le met a 0
            else if(lut[I[i][j]] > 255)
                actImg.at<uchar>(i, j) = 255;//si une valeur de pixel est supérieur a 255 on le met à 255
            else actImg.at<uchar>(i, j) = lut[I[i][j]];//On afecte la valeur correspondante du pixel de lut dans la matrice de l'image
        }
    for (int i = 0; i < img_h; ++i)
    {
        delete[] I[i];//libération de la mémoire
    }
    delete[] I;

    return actImg;//retour la matrice des pixels correspondante à l'image modifiée
}//Fin de la fonction getcontrastLinaireSaturation

Mat getContrastLinaireMorceaux(Mat actImg, float r1, float s1, float r2, float s2)
/*
    Objet           : Fonction permettant de calculer le contraste linéaire par morceaux d'une image
    Type de retour  : cv::Mat
*/
{

    if(r2 == r1 || r1 == 0 || r2 == 255)//si les valeurs des pixels sont non raisonable on ne modifi pas la matrice de l'image
        return actImg;
    int img_h = actImg.rows;
    int img_w = actImg.cols;
    uchar **I;
    I = new uchar*[img_h];
    for (int i = 0; i < img_h; ++i)
        I[i] = new uchar[img_w];

    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            I[i][j] = actImg.at<uchar>(i, j);
        }

    int T[NIVEAU_PIXEL_TOTAL];
    for (int r = 0; r < NIVEAU_PIXEL_TOTAL; ++r)//Boucle de calcul des nouvelles valeurs des pixels
    {
        if(r <= r1)
            T[r] = r*s1/r1;
        else if(r >= r2)
            T[r] = s2 +(r-r2)*(255-s2)/(255-r2);
        else
            T[r] = s1 + (r-r1)*(s2-s1)/(r2-r1);
    }

    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            actImg.at<uchar>(i, j) = T[I[i][j]];
        }
    for (int i = 0; i < img_h; ++i)
    {
        delete[] I[i];//libération de la mémoire
    }
    delete[] I;

    return actImg;//retour la matrice des pixels correspondante à l'image modifiée
}//Fin de la fonction getContrastLinaireMorceaux


Mat getContrastGama(Mat actImg, float gama)
/*
    Objet           : Fonction permettant de calculer les valeurs des nouvelles pixels correspondant au contraste gama d'une image
    Type de retour  : cv::Mat
*/
{

    int img_h = actImg.rows;
    int img_w = actImg.cols;
    int lut[NIVEAU_PIXEL_TOTAL];//Création d'un look up table
    uchar **I;
    I = new uchar*[img_h];
    for (int i = 0; i < img_h; ++i)
        I[i] = new uchar[img_w];

    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            I[i][j] = actImg.at<uchar>(i, j);
        }

    for (int i = 0; i < NIVEAU_PIXEL_TOTAL; ++i)
        lut[i] = calculGama(i, gama);//Calcul des nouvelles valeurs des pixels dans la lut

    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
            if (lut[I[i][j]] < 0)
                actImg.at<uchar>(i, j) = 0;//Mise a 0 de toutes les valeurs des pixels inféieurs à 0
            else if(lut[I[i][j]] > 255)
                actImg.at<uchar>(i, j) = 255;//Mise a 255 de toutes les valeurs supérieurs a 255
            else
                actImg.at<uchar>(i, j) = lut[I[i][j]];//Recherche de la nouvelle valeur du pixel dans la lut

    for (int i = 0; i < img_h; ++i)
        delete[] I[i];//libération de la mémoire
    delete[] I;

    return actImg;//retour la matrice des pixels correspondante à l'image modifiée
}//Fin de la fonction getContrastGama


Mat courbeFonctionGamma(float gamma)
/*
    Objet           : Fonction permettant de tracer la courbe de la fonction gama
    Type de retour  : cv::Mat
*/
{
    Mat imageFinal(256, 256, CV_8UC3, Scalar( 255,255,255));
    Point point1, point2;
    for (float i = 0.0; i<256.0; i++)
    {
        point1.x = i;
        point2.x = i+1;
        point1.y = 255.0 - (pow((point1.x/255.0),gamma)*255.0);
        point2.y = 255.0 - (pow((point2.x/255.0),gamma)*255.0);
        if(point1.y < 0.0)
            point1.y = 0.0;
        if(point1.y> 255.0)
            point1.y = 255.0;
        if(point2.y < 0.0)
            point2.y = 0.0;
        if(point2.y> 255.0)
            point2.y = 255.0;
        line(imageFinal, point1, point2, Scalar(255, 0, 0), 1, 8);
    }
    return imageFinal;//retour la matrice des pixels correspondante à la courbe de gama
}//Fin de la fonction courbeFonctionGamma


Mat courbeTraitementLineairePoints(Point pt1, Point pt2){

    Mat courbe(256,256,CV_8UC3,Scalar(255,255,255));

    Point debut, fin;
    debut.x = 0 ;
    debut.y = 255 ;
    fin.x = 255;
    fin.y = 0;
    pt1.y = 255 - pt1.y;
    pt2.y = 255 - pt2.y;

    line(courbe, debut, pt1, Scalar(255, 0, 0), 1, 8);
    line(courbe, pt1, pt2, Scalar(255, 0, 0), 1, 8);
    line(courbe, pt2, fin, Scalar(255, 0, 0), 1, 8);

    return courbe;
}

Mat getEgalisationHistogramme(Mat actImg)
/*
    Objet           : Fonction permettant de calculer le contraste par égalisation de l'histogramme d'une image
    Type de retour  : cv::Mat
*/
{
    int img_h = actImg.rows;
    int img_w = actImg.cols;
    int h[NIVEAU_PIXEL_TOTAL];
    float hn[NIVEAU_PIXEL_TOTAL];
    float c[NIVEAU_PIXEL_TOTAL];
    for (int i = 0; i < NIVEAU_PIXEL_TOTAL; i++) //initialisation des matrices à 0
    {
        h[i] = 0;
        hn[i] = 0;
        c[i] = 0;
    }

    int data_index = 0;

    for(int i = 0; i < img_h; i++)
        for (int j = 0; j < img_w; ++j)
        {
            data_index = actImg.at<uchar>(i,j);
            h[data_index]++;
        }

    int nbp = (actImg.cols)*(actImg.rows);
    for(int i = 0; i < NIVEAU_PIXEL_TOTAL; i++)
        hn[i] = (float)h[i]/(float)nbp;

    //Calcul de la densité
    for(int i = 0; i < NIVEAU_PIXEL_TOTAL; i++)
    {
        c[i] = 0.0;
        for(int j = 0; j <= i; j++)
            c[i] += hn[j];
    }

    int index = 0;
    for (int i = 0; i < img_h; ++i)
    {
        for (int j = 0; j < img_w; ++j)
        {
            index = actImg.at<uchar>(i, j);
            actImg.at<uchar>(i, j) = c[index]*255;//affectation des valeurs calculée dans la matrice de l'image modifié
        }
    }
    return actImg;//retour la matrice des pixels correspondante à l'image modifiée
}//Fin de la fonction getEgalisationHistogramme



void tracerProfil(string image_name, int x1, int y1, int x2, int y2)
/*
    Objet           : Fonction permettant de calculer le contraste par égalisation de l'histogramme d'une image
    Type de retour  : void
*/
{
    Mat image = imread(image_name, 1);
    Point start(x1, y1);
    Point end(x2, y2);
    float lengh = sqrt((pow ((x1-x2), 2)) + (pow ((y1-y2), 2))); //calcul de la ligne du profil d'intensité
    Mat profil_img(260, lengh + 10, CV_8UC3, Scalar(255, 255, 255));
    Mat hist(600, 256, CV_8UC3, Scalar(255, 255, 255));
    {
        vector<Mat> rgb_planes;
        split(image, rgb_planes );//division de l'image en ces niveau de gris
        for (int i = 0; i < image.channels(); i++)//traitement de chaque chanel dans l'image
        {
            float x_start, y_start;
            float x_end, y_end;
            float col, row;
            uchar data_start;
            uchar data_end;
            float dx, dy;
            float _sin, _cos;
            Point point_start, point_end;
            float lengh;
            x_start = start.x;
            y_start = start.y;
            x_end = end.x;
            y_end = end.y;
            dx = x_end - x_start;
            dy = y_end - y_start;
            lengh = sqrt(dx*dx + dy*dy);
            _sin = dy/lengh;
            _cos = dx/lengh;
            col = x_start;
            row = y_start;
            for( int p = 1; p < lengh; p++ )
            {
                data_start = rgb_planes[i].at<uchar>(row, col);
                row = y_start + _sin;
                col = x_start + _cos;
                data_end = rgb_planes[i].at<uchar>(row, col);
                point_start = Point(p, 255-data_start);
                point_end = Point(p-1, 255-data_end);
                line( profil_img, point_start, point_end, getScalar(i), 1, 8, 0 );
                y_start = row;
                x_start = col;
            }
        }
    }
    line(image, start, end, Scalar( 255, 0, 0), 1, 8, 0);

    vector<string> repVect = getImageFolder(image_name);
    imwrite(repVect.at(0)+repVect.at(1)+"_"+"image_out"+getIDNomImage()+".tif", image);
    imwrite(repVect.at(0)+repVect.at(1)+"_"+"profil"+getIDNomImage()+".tif", profil_img);
    cout<<"Resultats stockés dans dans le repertoire de l'image originale ("+repVect.at(0)+")\n";
}//Fin de la procedure tracerProfil



//Fonction d'affichage du message d'accueil
void Aide()
/*
    Objet           : Fonction permettant d'afficher l'aide
    Type de retour  : void
*/
{
    cout<<"Programme de traitement du contraste : auteur: PAZIMNA Solibia\n";
    cout<<"Syntaxe: module(pf ou cst) chemin_du_fichier_image parametres \n";
}//Fin de la procedure Aide



bool controlSaisie(vector<string> detailCmdUser)
/*
    Objet           : Fonction permettant de vérifier si la commande saisie par l'utilisateur est correcte
    Type de retour  : bool
*/
{
    //
    bool rep = true;
    if (detailCmdUser.size()==0){//Si aucun argument
        rep = false;
    }else if (detailCmdUser.at(0).compare("pf")!=0 && detailCmdUser.at(0).compare("cst")!=0) {//Si pas de module appelé
        rep = false;
    }else if (detailCmdUser.size()<2) {//Si pas de fichier image passé
        rep = false;
    }
    return rep;
}//Fin de la fonction controlSaisie



void OperationToDO(vector<string> detailCmdUser)
/*
    Objet           : Fonction permettant de déterminer le module correspondant a la commande
    Type de retour  : void
*/
{
    if (!controlSaisie(detailCmdUser))
    {
        cout<<"Syntaxe ou parametres de la commande incorectes \n";
    }
    else
    {
        if (detailCmdUser.at(0).compare("pf")==0 )//Si module profile
        {
            if (detailCmdUser.size()==6) {//vérification du nombre de parametres
                tracerProfil(detailCmdUser.at(1),std::stoi(detailCmdUser.at(2)), std::stoi(detailCmdUser.at(3)),
                             std::stoi(detailCmdUser.at(4)),std::stoi(detailCmdUser.at(5)));
            }else {
                cout<<"Parametres incorrectes\n";
            }

        }else if (detailCmdUser.at(0).compare("help")==0) {//Si aide
            //Aide
            Aide();
        }else{//Sinon lecture de l'image en parametre
            Mat image =   imread(detailCmdUser.at(1), -1);;//lecture de l'image
            Mat image_contrast = image.clone();//duplication de l'image lu
            Mat hist(600, 256, CV_8UC3, Scalar(255, 255, 255));
            Mat hist_contrast(600, 256, CV_8UC3, Scalar(255, 255, 255));

            vector<Mat> rgb_planes;
            split(image, rgb_planes);//Sépartation de l'image lu en ces différents channels
            int nbChanels = image.channels();

            for (int p=0; p<nbChanels; p++)//Traitement de chaque chanel contenu dans l'image lu
            {
                int nbArg = detailCmdUser.size();//Vérification du nombre de parametres pour savoir quel module de contraste appéllé
                tracerHistogramme(rgb_planes.at(p),getScalar(p),hist,p+1);//On trace l'histogramme de l'image original
                if (nbArg==2)
                {
                    //Transformation Egalisation de l'histogramme
                    image_contrast = getEgalisationHistogramme(rgb_planes.at(p));

                }
                else if (nbArg==3)
                {
                    //Transformation gama
                    image_contrast = getContrastGama(rgb_planes.at(p), std::stof(detailCmdUser.at(2)));
                    //imwrite(detailCmdUser.at(1)+getIDNomImage()+"fonctGama"+".tif", courbeFonctionGamma(std::stof(detailCmdUser.at(2))));//Tracer de la courbe gama
                    vector<string> repVect = getImageFolder(detailCmdUser.at(1));
                    imwrite(repVect.at(0)+repVect.at(1)+"_"+"fonctGama"+getIDNomImage()+".tif", courbeFonctionGamma(std::stof(detailCmdUser.at(2))));//Tracer de la courbe gama
                }
                else if (nbArg==4)
                {
                    //Transformation Linéaire par satudation
                    image_contrast = getcontrastLinaireSaturation(rgb_planes.at(p),std::stoi(detailCmdUser.at(2)),
                                     std::stoi(detailCmdUser.at(3)));

                }
                else if (nbArg==6)
                {
                    //Transformation Linéaire par morceau
                    image_contrast = getContrastLinaireMorceaux(rgb_planes.at(p),std::stof(detailCmdUser.at(2)),
                                     std::stof(detailCmdUser.at(3)),std::stof(detailCmdUser.at(4)),
                                     std::stof(detailCmdUser.at(5)));
                    //imwrite(detailCmdUser.at(1)+getIDNomImage()+"fonctGama"+".tif", courbeFonctionGamma(std::stof(detailCmdUser.at(2))));//Tracer de la courbe gama
                    vector<string> repVect = getImageFolder(detailCmdUser.at(1));
                    imwrite(repVect.at(0)+repVect.at(1)+"_"+"fonctLinMorceau"+getIDNomImage()+".tif",
                            courbeTraitementLineairePoints(Point(std::stof(detailCmdUser.at(2)),std::stof(detailCmdUser.at(3))),
                                                           Point(std::stof(detailCmdUser.at(4)),std::stof(detailCmdUser.at(5)))));//Tracer de la courbe linéaire

                }
                tracerHistogramme(rgb_planes.at(p),getScalar(p),hist_contrast,p+1);//Tracé de l'histogramme de l'image modifié
            }
            merge(rgb_planes, image_contrast);//Fusion de différent chanels séparé plus haut
            writeImage(hist,image_contrast,hist_contrast, detailCmdUser.at(1));//Appel de la fonction writeImage pour écrire les Mat obtenu dans un fichier
        }
    }
}//Fin de la fonction OperationToDO
