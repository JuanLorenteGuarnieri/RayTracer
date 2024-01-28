#include "photon_mapping.h"

extern vector<pLuz *> luces;

/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(vector<Photon> photons)
{
  PhotonMap map = PhotonMap(photons, PhotonAxisPosition());
  return map;
}

/*
    Example method to search for the nearest neighbors of the photon map
*/
void search_nearest(const PhotonMap &map, G_O p, unsigned long nPhotons, float radio, vector<const Photon *> &nearest)
{

  // nearest is the nearest photons returned by the KDTree
  nearest = map.nearest_neighbors(p, nPhotons, radio);
}

vector<float> pre_calc_pdfs_lights()
{
  vector<float> pdfs;

  Color sumPot;

  for (pLuz *luz : luces)
  {
    sumPot = sumPot + luz->color;
  }

  float sp = sumPot.maxi();

  for (size_t i = 0; i < luces.size(); i++)
  {
    pdfs.push_back(luces[i]->color.maxi() / sp);
  }

  return pdfs;
}

vector<int> pre_calc_nPhotons_lights(int MAX_PHOTONS)
{
  vector<int> nPhotonsLights;
  float sumPot;
  for (pLuz *luz : luces)
  {
    sumPot = sumPot + luz->color.maxi();
  }
  for (size_t i = 0; i < luces.size(); i++)
  {
    float pdf_light = luces[i]->color.maxi() / sumPot;
    nPhotonsLights.push_back((int)(MAX_PHOTONS * pdf_light));
  }
  return nPhotonsLights;
}

void select_light_source(int &ind, float &pdf, vector<float> pdfs)
{
  float p = ((rand() % 10000) / 10000.0f);
  ind = 0;
  float i = 0;
  float f = pdfs[ind];
  while (ind < luces.size() && !(i <= p && p <= f))
  {
    ind++;
    i = f;
    f = f + pdfs[ind];
  }
  pdf = pdfs[ind];
}

Rayo sample_outgoing_ray(G_O punto)
{
  //---------------------------------------------------------------------------------//
  // Calcular rayo aleatorio
  float e_inclinacion = ((rand() % 10000) / 10000.0f);
  float e_azimutal = (rand() % 10000) / 10000.0f;
  float inclinacion = acos(2 * e_inclinacion - 1);
  float azimutal = 2 * PI * e_azimutal;

  G_O aux = G_O(sin(inclinacion) * cos(azimutal), sin(inclinacion) * sin(azimutal),
                cos(inclinacion), 0);
  return Rayo(punto, aux);
  //---------------------------------------------------------------------------------//
}

void trace_ray(vector<Geometria *> geometrias, pLuz *luz, const int MAX_PHOTONS, const int MAX_SHOTS, vector<Photon> &photons, const float &pdf_light, bool typeDirectLight)
{
  vector<Photon> photonsAux;
  int iter = 0;
  int N = 0;
  int tipoColor = COLOR_CON_LUCES;
  bool count = typeDirectLight;
  while (iter < MAX_SHOTS && photons.size() + photonsAux.size() < MAX_PHOTONS)
  {
    Rayo rayo = sample_outgoing_ray(luz->origen);
    count = typeDirectLight;
    Color radianceInicial = luz->color * pdf_light * 4;
    Color radiance = radianceInicial;
    bool seguir = true;

    Rayo _rayo = rayo;
    G_O normalObject;
    Color objetoColor = Color(1, 1, 1);
    BRDF brdf = BRDF(BRDF_DIFUSO);
    while (seguir && radiance.maxi() > radianceInicial.maxi() / 10 && photons.size() + photonsAux.size() < MAX_PHOTONS)
    {
      bool isDelta = false;
      float _t = 1e10;
      luzGlobalPhoton(geometrias, _rayo, tipoColor, normalObject, objetoColor, radiance, brdf, _t, N, isDelta, seguir);
      N++;

      if (!isDelta && seguir && radiance.maxi() > radianceInicial.maxi() / 10 && photons.size() + photonsAux.size() < MAX_PHOTONS)
      {
        if (count == DL_USING_NE)
          count = true; // el siguiente choque del foton si se guarda
        else            // count == DL_USING_PM
        {
          G_O p = rayo.punto + rayo.direccion * _t;
          Photon ph = Photon(rayo.direccion, p, radiance);
          photonsAux.push_back(ph); // SET FLUX, POSITION AND INCOMING DIRECTION AT THAT POINT
        }
      }
      rayo = _rayo;
    }
    iter++;
  }
  Photon photon;
  while (!photonsAux.empty())
  {
    photon = photonsAux.back();
    photonsAux.pop_back();
    photon.potencia = photon.potencia / iter;
    photons.push_back(photon);
  }
}

void trace_ray2(vector<Geometria *> geometrias, pLuz *luz, const int MAX_ALL_PHOTONS, const int MAX_PHOTONS, vector<Photon> &photons, bool typeDirectLight)
{
  vector<Photon> photonsAux;
  int iter = 0;
  int N = 0;
  int tipoColor = COLOR_CON_LUCES;
  bool count = typeDirectLight;
  while (photonsAux.size() < MAX_PHOTONS && photons.size() + photonsAux.size() < MAX_ALL_PHOTONS)
  {
    N = 0;
    bool photonStored = false;
    Rayo rayo = sample_outgoing_ray(luz->origen);
    count = typeDirectLight;
    Color radianceInicial = luz->color * 4 * PI;
    Color radiance = radianceInicial;
    bool seguir = true;
    Rayo _rayo = rayo;
    G_O normalObject;
    Color objetoColor = Color(1, 1, 1);
    BRDF brdf = BRDF(BRDF_DIFUSO);
    while (seguir && radiance.maxi() > radianceInicial.maxi() / 10 && photons.size() + photonsAux.size() < MAX_ALL_PHOTONS)
    {
      bool isDelta = false;
      float _t = 1e10;
      luzGlobalPhoton(geometrias, _rayo, tipoColor, normalObject, objetoColor, radiance, brdf, _t, N, isDelta, seguir);
      N++;

      if (isDelta)
      {
        count = true;
      }

      if (!isDelta && seguir && radiance.maxi() > radianceInicial.maxi() / 10 && photons.size() + photonsAux.size() < MAX_ALL_PHOTONS)
      {
        if (count == DL_USING_NE)
          count = true; // el siguiente choque del foton si se guarda
        else            // count == DL_USING_PM
        {
          photonStored = true;
          G_O p = rayo.punto + rayo.direccion * _t;
          Photon ph = Photon(rayo.direccion, p, radiance);
          photonsAux.push_back(ph); // SET FLUX, POSITION AND INCOMING DIRECTION AT THAT POINT
        }
      }
      rayo = _rayo;
    }
    if (photonStored)
      iter++;
  }
  Photon photon;
  while (!photonsAux.empty())
  {
    photon = photonsAux.back();
    photonsAux.pop_back();
    photon.potencia = photon.potencia / N_BUSCAR_PHOTONS;
    photons.push_back(photon);
  }
}

/*
  MAX_SHOTS = Maximum number of photons shot per light
  MAX_PHOTONS = Maximum number of photon instances stored in photon map
*/
PhotonMap preprocess2(vector<Geometria *> geometrias, int MAX_PHOTONS, int MAX_SHOTS, bool typeDirectLight)
{
  vector<Photon> photons;
  vector<float> pdfs_lights = pre_calc_pdfs_lights();
  while (photons.size() < MAX_PHOTONS)
  {
    int i;
    float pdf_light;
    select_light_source(i, pdf_light, pdfs_lights);

    trace_ray(geometrias, luces[i], MAX_PHOTONS, MAX_SHOTS, photons, pdf_light, typeDirectLight);
    // What's the flux of each photon?? Light radiance, PDF of sampled ray, attenuation
  }

  // Modify flux of photons in all_photons based on NUMBER OF SHOTS TAKEN, not MAX_SHOTS
  return generation_of_photon_map(photons); // Build KD-tree, octree, list/vector, etc. using all_photons
}

PhotonMap preprocess(vector<Geometria *> geometrias, int MAX_PHOTONS, int MAX_SHOTS, bool typeDirectLight)
{
  vector<Photon> photons;
  vector<int> nPhotons_lights = pre_calc_nPhotons_lights(MAX_PHOTONS);
  for (size_t i = 0; i < luces.size(); i++)
  {
    trace_ray2(geometrias, luces[i], MAX_PHOTONS, nPhotons_lights[i], photons, typeDirectLight);
  }

  // Modify flux of photons in all_photons based on NUMBER OF SHOTS TAKEN, not MAX_SHOTS
  return generation_of_photon_map(photons); // Build KD-tree, octree, list/vector, etc. using all_photons
}

float gaussianKernel(float rp, float rk)
{
  float c = rk / sqrt(2 * PI); // Ajustar c como una función de rk
  return exp(-pow(rp, 2) / (2 * pow(c, 2)));
}

/*
  Este kernel es de forma parabólica y es más eficiente computacionalmente
  que el gaussiano. Se utiliza comúnmente en estadísticas para estimación de densidad.
*/
float epanechnikovKernel(float rp, float rk)
{
  float ratio = rp / rk;
  return max(0.0f, 1.0f - ratio * ratio);
}

float coneKernel(float rp, float rk)
{
  return rp / rk;
}

Color density_estimation(G_O intersection, Rayo rayo, G_O normal, vector<const Photon *> nearest_photons, float radio)
{
  Color L = Color(0, 0, 0);
  float rk = radio; /* algún valor basado en la implementación del algoritmo */
  G_O wi = {0, 0, 0, 0};
  for (const Photon *photon : nearest_photons)
  {
    wi = photon->wi;
    float rp = (intersection - photon->position_).modulo(); // Calcula la distancia al fotón
    // TODO añadir el calculo del BRDF
    Color pot = photon->potencia;

    Color aux = pot * abs(G_O::dot(normal, wi)) * gaussianKernel(rp, rk); // Angulo entre normal y dirección
                                                                          // La potencia del fotón

    L = L + aux;
  }
  return L;
}

Color shade(vector<Geometria *> geometrias, G_O &intersection, Rayo &rayo, float &_t, G_O &normal, bool isDelta, BRDF brdf, Color objetoColor, const PhotonMap &photon_map, float radio, int nPhotons)
{
  Rayo _rayo = rayo;
  bool seguir = true;
  int N = 1;
  while (isDelta && seguir)
  {
    _t = 1e10;
    trace_outgoing_ray(geometrias, _rayo, COLOR_CON_LUCES, normal, _t, N, isDelta, seguir, objetoColor);
    intersection = rayo.punto + rayo.direccion * _t;
    rayo = _rayo;
  }
  if (seguir)
  {
    vector<const Photon *> nearest_photons;
    search_nearest(photon_map, intersection, nPhotons, radio, nearest_photons);

    rayo.direccion.normalize();
    normal.normalize();
    Color L = density_estimation(intersection, rayo, normal, nearest_photons, radio);
    return L;
  }
  else
  {
    return Color(0, 0, 0);
  }
}
