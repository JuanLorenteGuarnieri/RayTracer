#ifndef PHOTON_MAPPING
#define PHOTON_MAPPING

#include "pLuz.h"
#include "./utils/luzGlobal.h"

/*
    The KD-Tree ready to work in 3 dimensions, with YourPhoton s, under a
    brand-new name: PhotonMap
*/
using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;

PhotonMap preprocess(vector<Geometria *> geometrias, int MAX_PHOTONS, int MAX_SHOTS, bool typeDirectLight);

Color shade(vector<Geometria *> geometrias, G_O &intersection, Rayo &rayo, float &_t, G_O &normal, bool isDelta, BRDF brdf, Color objetoColor, const PhotonMap &photon_map, float radio, int nPhotons);

#endif