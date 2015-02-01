#include "RayRenderer.hpp"
#include <OGRE/OgreRay.h>
#include <chrono>

using namespace std;

namespace rendering {


    RayRenderer::RayRenderer(std::shared_ptr<Scene> scene) : scene(scene),
                                                             MAX_DISTANCE(999999.0f),
                                                             NO_OBJECT(-1),
                                                             NO_COLOR(0.0, 0.0, 0.0, 1.0),
                                                             START_LEVEL(0),
                                                             START_COEF(1.0),
                                                             ANTYALIASING_FACTOR(scene->antyAliasing * scene->antyAliasing) {

    }

    void RayRenderer::render(Pixels *pixels) {
        float x, y;
        float x_flTmp, y_flTmp;
        float x_fbase, y_fbase; //
        float halfOfImageXSize = static_cast<float>(scene->dimension[0]) / 2;
        float halfOfImageYSize = static_cast<float>(scene->dimension[1]) / 2;
        Ogre::Vector3 starting_point;
        Ogre::Vector3 starting_direction(-scene->cameraZ[0], -scene->cameraZ[1], -scene->cameraZ[2]); // wektor opisuj�cy kierunek promienia

        x_fbase = static_cast<float>(scene->dimension[0]) / static_cast<float>(scene->viewportSize[0]);
        y_fbase = static_cast<float>(scene->dimension[1]) / static_cast<float>(scene->viewportSize[1]);
        Ogre::Plane plane(scene->cameraX, scene->cameraY, scene->cameraZ);
        float dist = plane.getDistance(scene->cameraPosition);

        Ogre::ColourValue pixel(0.0, 0.0, 0.0, 1.0);
        Ogre::ColourValue finalPixel(0.0, 0.0, 0.0, 1.0);
        float d = MAX_DISTANCE;

        Ogre::Ray ray(starting_point, starting_direction);
        const int maxBounceRate = scene->maxRayBounce;
        int currentObjectId = NO_OBJECT;
        int currentLevel = START_LEVEL;
        float coef = START_COEF;

        Ogre::Vector3 normalVec;
        const float sampleRatio = 1 / ANTYALIASING_FACTOR;
        const float sampleDiff = 1 / Ogre::Math::Sqrt(ANTYALIASING_FACTOR);

        for (y = halfOfImageYSize - float(scene->window[0][1]); y > halfOfImageYSize - float(scene->window[1][1]); --y) {
            for (x = -halfOfImageXSize + float(scene->window[0][0]); x < -halfOfImageXSize + float(scene->window[1][0]); ++x) {

                finalPixel = NO_COLOR;

                for (float fragmentx = static_cast<float>(x); fragmentx < x + 1.0f; fragmentx += sampleDiff)
                    for (float fragmenty = static_cast<float>(y); fragmenty < y + 1.0f; fragmenty += sampleDiff) {
                        coef = START_COEF;
                        currentLevel = START_LEVEL;

                        x_flTmp = fragmentx / x_fbase;
                        y_flTmp = fragmenty / y_fbase;


                        pixel = NO_COLOR;

                        starting_point[0] = scene->cameraPosition[0] + (x_flTmp * scene->cameraX[0] + y_flTmp * scene->cameraY[0]) - dist * scene->cameraZ[0];
                        starting_point[1] = scene->cameraPosition[1] + (x_flTmp * scene->cameraX[1] + y_flTmp * scene->cameraY[1]) - dist * scene->cameraZ[1];
                        starting_point[2] = scene->cameraPosition[2] + (x_flTmp * scene->cameraX[2] + y_flTmp * scene->cameraY[2]) - dist * scene->cameraZ[2];// transformacja wsp kamery na globalne


                        ray.setOrigin(scene->cameraPosition);

                        ray.setDirection(starting_point - scene->cameraPosition);
                        do {
//                            currentObjectId = findIntersectingObject(scene->objects, ray);
                            currentObjectId = NO_OBJECT;

                            d = MAX_DISTANCE;


                            for (unsigned int g = 0; g < scene->objects.size(); g++) {
                                std::pair<bool, float> isIntersecting = scene->objects[g]->intersects(ray);
                                assert(scene->objects[g]->getId() == g);
                                if (isIntersecting.first && isIntersecting.second < d) {
                                    d = isIntersecting.second;
                                    currentObjectId = g;
                                }
                            }

                            if (currentObjectId == NO_OBJECT) {
                                pixel += scene->background;
                                break;
                            }

                            const auto &currentObject = scene->objects[currentObjectId];

                            Ogre::Vector3 newStart = ray.getPoint(d);
                            normalVec = currentObject->getNormalVector(newStart);
                            normalVec.normalise();


                            if (currentLevel == 0) {
                                pixel += currentObject->getAmbient() * scene->globalLight;
                            }
                            for (const auto &light : scene->lights) {
                                std::pair<bool, float> isIntersecting = light->intersects(ray);
                                if (isIntersecting.first && isIntersecting.second < d) {
                                    pixel = light->getAmbient();
                                    break;
                                }
                                ////////////
                                std::shared_ptr<Light> current = light;

                                Ogre::Vector3 lightVec = current->getSphere().getCenter() - newStart;
                                lightVec.normalise();
                                Ogre::Ray lightRay(newStart, lightVec);

                                bool inShadow = false;
                                for (auto &elem : scene->objects) {
                                    std::pair<bool, float> isIntersecting = elem->intersects(lightRay);
                                    if (isIntersecting.first) {
                                        inShadow = true;
                                        break;
                                    }
                                }
                                if (!inShadow) {
                                    const auto &blinnDirection = calculateBlinnDirection(lightRay, ray);
                                    auto blinnValue = calculateBlinnValue(blinnDirection, normalVec, currentObject, coef);
                                    pixel += countBlinnOffset(blinnValue, current.operator*(), currentObject);

                                    const auto &phongDirection = calculatePhongDirection(lightRay, normalVec);
                                    auto phongValue = calculatePhongValue(phongDirection, ray, currentObject, coef);
                                    pixel += calculatePhongOffset(phongValue, currentObject, current.operator*());

                                    pixel += current->getAmbient() * currentObject->getAmbient();
                                    coef *= currentObject->getN();
                                }

                            }

                            // We iterate on the next reflection

                            float rayReflect = 2.0f * (ray.getDirection().dotProduct(normalVec));
                            ray.setOrigin(newStart);
                            ray.setDirection(ray.getDirection() - ((rayReflect) * normalVec));

                            currentLevel++;

                        } while ((coef > 0.001f) && currentLevel < maxBounceRate);
                        finalPixel += pixel * sampleRatio;
                    }
                pixels->push_back(static_cast<unsigned char>(round((finalPixel.r > 1.0 ? 1.0 : finalPixel.r) * 255)));
                pixels->push_back(static_cast<unsigned char>(round((finalPixel.g > 1.0 ? 1.0 : finalPixel.g) * 255)));
                pixels->push_back(static_cast<unsigned char>(round((finalPixel.b > 1.0 ? 1.0 : finalPixel.b) * 255)));
            }
        }
    }

    ObjectId RayRenderer::findIntersectingObject(
            const MaterialObjectPtrVec &sceneObjects, const Ogre::Ray &ray) const {
        using Distance = float;
        using IsIntersecting = bool;
        using IntersectsDistancePair = std::pair<IsIntersecting, Distance>;
        using ObjectIdToIntersectsDistancePairMap = std::map<ObjectId, Distance>;

        ObjectIdToIntersectsDistancePairMap intersectingObjInformation =
                std::accumulate(std::begin(sceneObjects), std::end(sceneObjects), ObjectIdToIntersectsDistancePairMap(),
                        [&ray](const ObjectIdToIntersectsDistancePairMap &intersectionInformations, const MaterialObjectPtrVec::value_type &object)
                                -> ObjectIdToIntersectsDistancePairMap {
                            IntersectsDistancePair intersectionInformation = object->intersects(ray);
                            if (intersectionInformation.first) {
                                ObjectIdToIntersectsDistancePairMap newIntersectionInformations = intersectionInformations;
                                newIntersectionInformations[object->getId()] = intersectionInformation.second;
                                return newIntersectionInformations;
                            }
                            return intersectionInformations;
                        });

        if (!intersectingObjInformation.empty()) {
            auto closestIntersecting
                    = *min_element(std::begin(intersectingObjInformation), std::end(intersectingObjInformation),
                            [](const ObjectIdToIntersectsDistancePairMap::value_type &lhs,
                                    const ObjectIdToIntersectsDistancePairMap::value_type &rhs) {
                                return lhs.second < rhs.second;
                            });
            return closestIntersecting.first;
        }
        else {
            return NO_OBJECT;
        }
    }

    Ogre::ColourValue RayRenderer::calculatePhongOffset(
            const float phongValue, const MaterialObjectPtr &currentObject, const Light &currentLight) {
        return phongValue * currentLight.getDiffuse() * currentObject->getDiffuse();
    }

    Ogre::Vector3 RayRenderer::calculatePhongDirection(const Ogre::Ray &lightRay, const Ogre::Vector3 &normalVec) const {
        float lighRayReflect = 2 * lightRay.getDirection().dotProduct(normalVec);
        return lightRay.getDirection() - (lighRayReflect * normalVec);
    }

    float RayRenderer::calculatePhongValue(const Ogre::Vector3 &phongDirection, const Ogre::Ray &ray,
            const MaterialObjectPtr &currentObject, const float coeficiency) const {
        float phongTerm = max(phongDirection.dotProduct(ray.getDirection()), 0.0f);
        return currentObject->getN() * Ogre::Math::Pow(phongTerm, currentObject->getPhongN()) * coeficiency;
    }

    Ogre::ColourValue RayRenderer::countBlinnOffset(const float blinnValue,
            const Light &currentLight, const MaterialObjectPtr &currentObject) {
        return blinnValue * currentLight.getSpecular() * currentObject->getSpecular();
    }

    Ogre::Vector3 RayRenderer::calculateBlinnDirection(const Ogre::Ray &lightRay, const Ogre::Ray &ray) const {
        const auto &blinnDirection = lightRay.getDirection() - ray.getDirection();
        return (1.0f / (blinnDirection.length() + 0.000001f)) * blinnDirection;
    }

    float RayRenderer::calculateBlinnValue(const Ogre::Vector3 &blinnDir, const Ogre::Vector3 &normalVec,
            const MaterialObjectPtr &currentObject, const float coeficiency) const {
        float blinn = std::max(blinnDir.dotProduct(normalVec), 0.0f);
        return Ogre::Math::Pow(blinn, currentObject->getBlinnN()) * currentObject->getN() * coeficiency;
    }

} //namespace rendering