/*
Description :   This is an example of usage of the KDTree class. It does not
                compile, but should give you the idea of how to integrate the
                KDTree in your code
*/

#include "KdTree.h"

/*
    Your Photon class implementation, which stores each
    photon walk interaction
*/
class YourPhoton
{
public:
    G_O position_; // 3D point of the interaction
    G_O wi;
    Color potencia;
    // It returns the axis i position (x, y or z)
    float position(std::size_t i) const
    {
    }
};

/*
    An additional struct that allows the KD-Tree to access your photon position
*/
struct PhotonAxisPosition
{
    float operator()(const YourPhoton &p, std::size_t i) const
    {
        if (i == 1)
        {
            return p.position_.x;
        }
        else if (i == 2)
        {
            return p.position_.y;
        }
        else if (i == 3)
        {
            return p.position_.z;
        }
    }
};

/*
    The KD-Tree ready to work in 3 dimensions, with YourPhoton s, under a
    brand-new name: YourPhotonMap
*/
using YourPhotonMap = nn::KDTree<YourPhoton, 3, PhotonAxisPosition>;

/*
    Example function to generate the photon map with the given photons
*/
YourPhotonMap generation_of_photon_map(...)
{
    vector<YourPhoton> photons; // Create a list of photons
    YourPhotonMap map = YourPhotonMap(photons, PhotonAxisPosition());
    return map;
}

/*
    Example method to search for the nearest neighbors of the photon map
*/
void search_nearest(YourPhotonMap map, G_O p, unsigned long nPhotons, float radio, auto nearest)
{

    // nearest is the nearest photons returned by the KDTree
    nearest = map.nearest_neighbors(p, nPhotons, radio);
}
